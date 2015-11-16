//**************************************************************
//Program name:        linker.cpp
//Assignment:          Lab1
//Author:              Subhankari Mishra
//Due Date:            02/17/2015
//**************************************************************

//*******************Library inclusions*************************
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <exception>
//**************************************************************
using namespace std;

//*******************Global variable declarations***************
int static  LineCount,      // current line number
            Offset,         // current line Offset to read characters
            st_counter,     // current index for symbol table
            last_offset,    // last position of the valid character to be used for parse errors
            input_length,   // number of characters of current line of the file
            tot_num_instr,  // total number of instructions executed / of the program
            tot_num_sym,    // total number of symbols defined
            space_count,    // number of spaces between / after tokens in the same line
            mod_count,      // current module number
            //instr_count,    // number of instructions in current module
            usecount,       // number of symbols used in the current module
            warn_count,     // counter to track the number of warnings
            base_address;   // bade address of current module

bool        string_set,     // gets set when a token is identified
            build = false;  // indicator for the second pass

string      instr_type,     //current instruction type
            sub,            // current token
            line;           //current line from file being processed
//******************************************************************

//******************Structure Declarations**************************
struct  last_state{             //stores the last state when a function
        string  method;          //of a module is exited to read the next line;
        string  sub_method;      //used to resume the process from the last state
        int     counter;
        int     max_counter;
        }ls;

struct  sym_table{              //stores the symbols defined in the modules and
        string  symbol;          //additional data for processing
        int     rel_add;
        string  Error;
        bool    used;
        int     module;
        int     actual_add;
        }st[255];

struct  uselist{                //stores the symbols used in the current module
        string  used_sym;
        bool    used;
        }ul[16];

struct  map_table{              //stores the memory map with additional data for processing
        int     base_address;
        int     instr_num;
        string  address;
        string  instr_type;
        string  instr;
        string  symbol;
        string  final_address;
        string  error_status;
        int     error_num;
        string  err_symbol;
        int     m_count;
        }mt[512];

struct  warning{                //stores details of warning messages
        int     errcode;
        string  sym;
        int     m_count;
        int     add;
        int     m_size;
        }warn[512];

//******************************************************************

//**********************Function Declaration and Definitions********

//called to catch syntax errors
void ParseError(int errcode){

    string static errstr[] = {
            "NUM_EXPECTED",            // Number expect
            "SYM_EXPECTED",            // Symbol Expected
            "ADDR_EXPECTED",           // Addressing Expected
            "SYM_TOLONG",              // Symbol Name is to long
            "TO_MANY_DEF_IN_MODULE",   // > 16
            "TO_MANY_USE_IN_MODULE",   // > 16
            "TO_MANY_INSTR",           // total num_instr exceeds memory size (512)
            };

    cout<<"Parse Error line "<<LineCount<<" offset "<<last_offset<<": "<< errstr[errcode];

};

//called to catch error messages while building the memory map
void Error(int errcode, string symbol){

    string static errstr1[] = {
            "Absolute address exceeds machine size; zero used",
            "Relative address exceeds module size; zero used",
            "External address exceeds length of uselist; treated as immediate",
            " is not defined; zero used",
            "This variable is multiple times defines; first value used",
            "Illegal immediate value; treated as 9999",
            "Illegal opcode; treated as 9999"
            };
    if((symbol == "") && (errcode != 3)){
        cout<<" Error: " <<errstr1[errcode];
    }else{
        cout<<" Error: "<<symbol<<errstr1[errcode];
    };

};

//called to catch warning messages while building the memory map
void Warning(int errcode, int m_count, string sym, int add, int m_size){

    string static errstr2[] = {
            " assume zero relative",
            " appeared in the uselist but was not actually used",
            " was defined but never used"
            };

    if(errcode == 0){
        cout<<"Warning: Module "<<m_count<<": "<<sym<<" to big "<<add<<" (max="<<m_size<<")"<<errstr2[errcode]<<"\n";
    }else{
        cout<<"\n"<<"Warning: Module "<<m_count<<": "<<sym<<errstr2[errcode];
    };

};

////**************************Functions to validate the tokens*************
//validates a counter(demical digits)
//like defcount, usecount, instrcount, relative address
int accept_counter(string sub) throw(int){

    int counter = 0;
    int j = sub.length();
    stringstream ss;

    ss<<sub;
    ss>>counter;

    for( int i = 0; i < j; i++){
        if(!(isdigit(sub[0]))){          //check if the character is a decimal digit (0-9)
            throw (0);                   // NUM_EXPECTED
        };
    };
    return counter;

};

//validates a symbol (alpha[alphanum]*) (lenght < 16)
void accept_sym(string sub) throw(int){

    int i = sub.length();

    if(i > 16){                         //check if the symbol has more than 16 characters
        throw (3);                      //SYM_TOLONG
    };

    i = 0;

    while (sub[i]){
        if (i == 0){
            if(!(isalpha(sub[i]))){     //check if the first character is alphabetic
                throw (1);              //SYM_EXPECTED
            };
        }else{
            if(!(isalnum(sub[i]))){     //check if the subsequent characters are alphanumeric
                throw (1);              //SYM_EXPECTED
            };
        };
        i++;
    };
};

//Validate instruction type (I/A/R/E)
void accept_instr_type(string sub) throw(int){

    int i = sub.length();

    if (i > 1){                         //instruction type should be a single character
        throw (2);                      //ADDR_EXPECTED
    };

    if ( (sub != "I") &&
         (sub != "A") &&
         (sub != "R") &&
         (sub != "E") ){
        throw (2);                      //ADDR_EXPECTED
    };
};

//validate instructions (should be all decimal digits)
void accept_instr(string sub) throw(int){

    int i = sub.length();

    for ( int j =0; j < i; j++){
        if (!(isdigit(sub[j]))){
            throw (0);                 //NUM_EXPECTED
        };
    };
};
////*****************************************************************

////*****************Functions for data processing*******************
//Reads the next token from the line
//raises exception to read next line from the file on reaching end of line
 string get_next_token(string input) throw(int){

    bool string_set = false;
    string sub  = "",
           sub1 = "";
    int i= 0;

    try{
        while(!string_set){                         //loop/reach characters until a word/token is identified
            if ((input_length) == (Offset)){        //at end of line raise exception to read the next line
                throw (0);
            };

            if(!isspace(input[Offset])){            //if the character is not space

                if(i == 0){
                    last_offset = Offset + 1;       //set last characters position as the current position
                };

                i++;
                space_count = 0;
                sub1 = input[Offset];
                sub = sub + sub1;                   //form the word by concatenating the characters
                Offset++;

                if ((input_length) == (Offset)){    //if the previous character was the last character of the line
                                                    //then return the token/word
                    if (last_offset == 1){
                        last_offset = last_offset + 1;
                    };

                    string_set = true;
                    return sub;
                }
            }
            else{
                                                    //space marks the end of a word
                space_count = space_count + 1;
                Offset++;

                if ((space_count == 1) && (Offset != 1)){       //return the word upon encountering the first space after the character(s)
                    string_set = true;
                    return sub;
                };
            };
        }
    }
    catch(int error){
    throw (0);                                      //read the next line
    };

 };

//Build the memory map
 void build_map_table(int base_address, int instr_count, string instr, string instr_type, int m_size){

    int     i      = base_address + instr_count,
            add_ln = 0,
            j      = 0,
            address;
     string opcode,
            str_a,
            temp_str;
     bool   sym_set = false;

    mt[i].base_address = base_address;
    mt[i].instr_num = instr_count;

    ostringstream convert;   // stream used for the conversion
    convert << i;

    mt[i].address = convert.str();
    mt[i].m_count = mod_count;

    add_ln = mt[i].address.length();
    if(add_ln < 3){
        j = 3 - add_ln;
        for(int k = 0; k < j; k++){
            mt[i].address = "0" + mt[i].address;
        };
    };
    mt[i].instr_type = instr_type;
    mt[i].instr = instr;

//if the instruction type is not Immediate and the length of the instruction is greater than 4
    if((instr.length() > 4) && (instr_type != "I")){
        mt[i].final_address = "9999";
        mt[i].error_status  = "E";
        mt[i].error_num     = 6;                        //"Illegal opcode; treated as 9999"
        mt[i].err_symbol    = "";
     }
     else{
            //logic for instruction type I
        if(instr_type == "I"){
            if(instr.length() > 4){
                mt[i].final_address = "9999";
                mt[i].error_status  = "E";
                mt[i].error_num     = 5;                //"Illegal immediate value; treated as 9999"
                mt[i].err_symbol    = "";
            }
            else{
                add_ln = instr.length();
                    if(add_ln < 4){
                        j = 4 - add_ln;
                        for(int k = 0; k < j; k++){
                            instr = "0" + instr;
                        };
                    };
                mt[i].final_address = instr;
            };
        };

        opcode = instr[0];
        str_a = instr.substr(1,(instr.length() - 1));
        stringstream ss1;
        ss1<<str_a;
        ss1>>address;

        //logic for instruction type A
        if(instr_type == "A"){

            if(address >= 512){
                mt[i].final_address = opcode + "000";
                mt[i].error_status = "E";
                mt[i].error_num = 0;                    //"Absolute address exceeds machine size; zero used"
                mt[i].err_symbol = "";
            }
            else{
                mt[i].final_address = instr;
            };

        };
        //logic for instruction type R
        if(instr_type == "R"){

            if(address > m_size){
                address = base_address;
                ostringstream convert1;
                convert1 << address;
                temp_str = convert1.str();
                add_ln = temp_str.length();

                if(add_ln < 3){
                    j = 3 - add_ln;
                    for(int k = 0; k < j; k++){
                        temp_str = "0" + temp_str;
                    };
                };

                mt[i].final_address = opcode + temp_str;
                mt[i].error_status = "E";
                mt[i].error_num = 1;                                //"Relative address exceeds module size; zero used"
                mt[i].err_symbol = "";
            }
            else{
                address = base_address + address;
                ostringstream convert2;
                convert2 << address;
                temp_str = convert2.str();
                add_ln = temp_str.length();

                if(add_ln < 3){
                    j = 3 - add_ln;
                    for(int k = 0; k < j; k++){
                        temp_str = "0" + temp_str;
                    };
                };
                mt[i].final_address = opcode + temp_str;
            };
        };

        //logic for instruction type E
        if(instr_type == "E"){
            if(address > (usecount - 1)){
                temp_str = str_a;
                mt[i].final_address = opcode + temp_str;
                mt[i].error_status = "E";
                mt[i].error_num = 2;                        //"External address exceeds length of uselist; treated as immediate"
                mt[i].err_symbol = "";
            }
            else{
                mt[i].symbol = ul[address].used_sym;
                ul[address].used = true;
                j = 0;
                while((!sym_set) && (j < tot_num_sym)){
                    if (st[j].symbol == mt[i].symbol){
                        address = st[j].rel_add;
                        st[j].used = true;
                        if ((st[j].Error == "E") && (st[j].module == mod_count)){
                            mt[i].error_status = "E";
                            mt[i].error_num = 4;                //"This variable is multiple times defines; first value used"
                            mt[i].err_symbol = "";
                        };
                        sym_set = true;
                    };
                    j++;
                };
                if(sym_set){
                    ostringstream convert3;
                    convert3 << address;
                    temp_str = convert3.str();
                    add_ln = temp_str.length();
                    if(add_ln < 3){
                        j = 3 - add_ln;
                        for(int k = 0; k < j; k++){
                            temp_str = "0" + temp_str;
                        };
                    };
                    mt[i].final_address = opcode + temp_str;
                }else{
                    mt[i].final_address = opcode + "000";
                    mt[i].error_status = "E";
                    mt[i].error_num = 3;                    //" is not defined; zero used",
                    mt[i].err_symbol = mt[i].symbol;
                };
            };
        };
    };
};

//clears the last state once the process is resumed
void clear_ls(){
    ls.method = " ";
    ls.sub_method = " ";
    ls.counter = 0;
    ls.max_counter = 0;
};

//Reads the definition list
bool ReadDefList(string input) throw(int){
    string line,
           sub,
           sub1;
    int defcount,
        i,
        rel_add;
    bool sym_exists   = false;

    try{
        if ((ls.method == " ") || (ls.sub_method == "accept_defcount")){
            try{
                sub = get_next_token(input);
            }
            catch(int error){
                ls.method = "ReadDefList";
                ls.sub_method = "accept_defcount";
                return false;
            };
            defcount = accept_counter(sub);
            if(defcount > 16){
                throw(4);
            };
            clear_ls();
        }
        else{
            defcount = ls.max_counter;
        };

        for( i = 0; i < defcount; i++ )
        {
            sym_exists = false;
            if(ls.method != " "){
                i = ls.counter;
            };
            if((ls.sub_method == "accept_sym")||(ls.sub_method == " ")){
                try{
                    sub = get_next_token(input);
                }
                catch(int error){
                    ls.method = "ReadDefList";
                    ls.sub_method = "accept_sym";
                    ls.counter = i;
                    ls.max_counter = defcount;
                    return false;
                };
                accept_sym(sub);

                if(!build){
                    for(int i = 0 ; i < st_counter; i++){
                        if (st[i].symbol == sub){
                            st[i].Error = "E";
                            sym_exists = true;
                        };
                    };
                    if (!(sym_exists)){
                        st[st_counter].symbol = sub;
                        st[st_counter].used = false;
                        st[st_counter].module = mod_count;
                    };
                };
                clear_ls();
            }

            if((ls.sub_method == "accept_rel_add") || (ls.sub_method == " ")){
                try{
                    sub = get_next_token(input);
                }
                catch(int error){
                    ls.method = "ReadDefList";
                    ls.sub_method = "accept_rel_add";
                    ls.counter = i;
                    ls.max_counter = defcount;
                    return false;
                };
                rel_add = accept_counter(sub);
                if(!build){
                    if(!sym_exists){
                        st[st_counter].actual_add = rel_add;
                        st[st_counter].rel_add = rel_add + tot_num_instr;
                        tot_num_sym = st_counter + 1;
                        st_counter++;
                    };
                };
                clear_ls();
            };
        };
        return true;
    }
    catch(int error_code){
    throw (error_code);
    };
};

//Reads the use list
bool ReadUseList(string input) throw(int){
    string line,
           sub;
    int i;

    usecount = 0;

    try{
        if((ls.method == " ") || (ls.sub_method == "accept_usecount")){
            try{
                sub = get_next_token(input);
            }
            catch(int error){
                ls.method = "ReadUseList";
                ls.sub_method = "accept_usecount";
                return false;
            };
            usecount = accept_counter(sub);

            if(usecount > 16){
              throw (5);
            };

            clear_ls();
        }
        else{
            usecount = ls.max_counter;
        };

        for( i = 0; i < usecount; i++)
        {
            if(ls.method != " "){
                i = ls.counter;
            };
            if ((ls.sub_method == " ") || (ls.sub_method == "accept_sym")){
            try{
                sub = get_next_token(input);
            }
            catch(int error){
                ls.method = "ReadUseList";
                ls.sub_method = "accept_sym";
                ls.counter = i;
                ls.max_counter = usecount;
                return false;
            };
            accept_sym(sub);

            if(build){
                ul[i].used_sym = sub;
                ul[i].used = false;
            };
            clear_ls();
            };
        };
        return true;
    }
    catch(int error_code){
    throw (error_code);
    };
};

bool ReadInstList(string input) throw(int){
    string line,
           sub,
           sym,
           address;
    int instrcount,
        i,
        m_size,
        add;

    try{
        if((ls.method == " ") || (ls.sub_method == "accept_instrcount")){
            try{
            sub = get_next_token(input);
            }
            catch(int error){
                ls.method = "ReadInstList";
                ls.sub_method = "accept_instrcount";
                return false;
            };

            instrcount = accept_counter(sub);

            if(build){
                base_address = tot_num_instr;
            };

            tot_num_instr = tot_num_instr + instrcount;

            if (tot_num_instr > 512){
                throw(6);
            };
            clear_ls();
        }
        else{
            instrcount = ls.max_counter;
        };

        if(!build){
            if(tot_num_sym > 0){
                for(int l = 0; l < tot_num_sym; l++){
                    if(st[l].module == mod_count){
                        if(st[l].actual_add >= instrcount){
                            st[l].Error = "W";
                            st[l].rel_add = 0;
                            m_size = instrcount - 1;
                            sym = st[l].symbol;
                            add = st[l].actual_add;
                            Warning(0, mod_count, sym, add, m_size);
                        };
                    };
                };
            };
        };

        for( i = 0; i < instrcount; i++)
        {
            if(ls.method != " "){
                i = ls.counter;
            };
            if((ls.sub_method == "accept_instr_type") || (ls.sub_method == " ")){
                try{
                    sub = get_next_token(input);
                }
                catch(int error){
                    ls.method = "ReadInstList";
                    ls.sub_method = "accept_instr_type";
                    ls.counter = i;
                    ls.max_counter = instrcount;
                    return false;
                };

                accept_instr_type(sub);

                if(build){
                    instr_type = sub;
                };
                clear_ls();
            };

            if((ls.sub_method == "accept_instr") || (ls.sub_method == " ")){
                try{
                    sub = get_next_token(input);
                }
                catch(int error){
                    ls.method = "ReadInstList";
                    ls.sub_method = "accept_instr";
                    ls.counter = i;
                    ls.max_counter = instrcount;
                    return false;
                };

                accept_instr(sub);

                if(build){
                    build_map_table(base_address,i,sub,instr_type,instrcount);
                };
                clear_ls();
            };
        };
        return true;
    }
    catch(int error_code){
    throw (error_code);
    };
};

void ReadFile(string filename) throw(int){
    string line,
           sym,
           method;
    bool set_deflist  = false,
         set_uselist  = false,
         set_instlist = false,
         skip         = false;

    int  num_line     = 0;

    ifstream file;
    mod_count = 1;
    try{
        file.open(filename.c_str());
        if (file.is_open())
        {
            while(!file.eof()){
            //********************Create Module***************
                if(input_length == Offset){
                    getline(file,line);
                    skip = false;
                    input_length = line.length();
                    LineCount = LineCount + 1;
                }
                else{
                    skip = true;
                };

                if (input_length == 0){
                    last_offset = Offset + 1;
                    Offset = 0;
                    num_line = num_line + 1;
                    if (num_line > 1)
                    {
                        last_offset = 1;
                    };
                };

                if (input_length != 0){
                    num_line = 0;
                    if(!skip){
                        Offset = 0;
                        last_offset = 0;
                    };
                    if(!set_deflist){
                        set_deflist = ReadDefList(line);
                    };
                    if((!set_uselist) && (set_deflist)){
                        set_uselist = ReadUseList(line);
                    };
                    if((!set_instlist) && (set_uselist)){
                        set_instlist = ReadInstList(line);
                    };
                    if ((set_instlist)&&(set_deflist)&&(set_uselist)){
                        if(build){
                            for(int l = 0; l < usecount; l++){
                                if(ul[l].used == false){
                                    sym = ul[l].used_sym;
                                    warn[warn_count].errcode = 1;
                                    warn[warn_count].sym = sym;
                                    warn[warn_count].m_count = mod_count;
                                    warn[warn_count].add = 0;
                                    warn[warn_count].m_size = 0;
                                    warn_count++;
                                };
                            ul[l].used_sym = "";
                            ul[l].used = false;
                            };
                        };
                        mod_count++;
                        set_deflist = false;
                        set_uselist = false;
                        set_instlist = false;
                    };
                };
//*********************************End Module*************************************************
            };
            if(((!set_deflist) || (!set_uselist) || (!set_instlist) )){
                LineCount = LineCount - 1;
                if ((ls.method != " ") && (ls.sub_method != " ")){
                    method = ls.sub_method;
                    if((ls.sub_method == "accept_defcount") || ls.sub_method == "accept_rel_add" ||
                        ls.sub_method == "accept_usecount" || ls.sub_method == "accept_instrcount" ||
                        ls.sub_method == "accept_instr"){
                            throw (0);
                    };
                    if(ls.sub_method == "accept_sym"){
                        throw (1);
                    };
                    if(ls.sub_method == "accept_instr_type"){
                        throw (2);
                    };
                };
              };
          };
//*****************************************************
        file.close();
    }
    catch(int error_code){
    throw (error_code);
    };
};

//print symbol table
void Print_symbol_table(){
    cout<<"Symbol Table";
    for(int i = 0; i < tot_num_sym; i++){
        cout<<"\n"<<st[i].symbol<<"="<<st[i].rel_add;
        if(st[i].Error == "E")
        {
            cout<<" Error: This variable is multiple times defined; first value used";
        }
    };
};

//print memory map
 void print_map_table(){
     int errcode,
         m_count,
         add,
         m_size;
    string sym;

    for(int i = 0; i < tot_num_instr; i++){
        cout<<"\n"<<mt[i].address<<": "<<mt[i].final_address;
        if(mt[i].error_status == "E"){
            Error(mt[i].error_num,mt[i].err_symbol);
        };

        if((mt[i].m_count != mt[i + 1].m_count)){
            for(int j = 0; j < warn_count; j++){
                if(mt[i].m_count == warn[j].m_count){
                    errcode = warn[j].errcode;
                    sym = warn[j].sym;
                    add = warn[j].add;
                    m_size = warn[j].m_size;
                    m_count = warn[j].m_count;
                    Warning(errcode,m_count,sym,add,m_size);
                };
            };
        };
    };
    cout<<"\n";
    for(int i = 0; i < tot_num_sym; i++){
        if(st[i].used == false){
            errcode = 2;
            sym = st[i].symbol;
            add = 0;
            m_size = 0;
            m_count = st[i].module;
            Warning(errcode, m_count, sym, add, m_size);
        };
    };
};

void build_table(string filename){
    ReadFile(filename);
    print_map_table();
};
//***************************************************************************

//***************************Main Function***********************************
int main(int argc, char** argv ){

    string filename = argv[1];

    warn_count = 0;
    clear_ls();
//*********************Parser: 1st Pass**************************************
    try{
        ReadFile(filename);

        Print_symbol_table();
//****************Build Memory Map : 2nd Pass********************************
        build = true;
        cout<<"\n"<<"\n"<<"Memory Map";
        if(tot_num_instr!= 0){
            tot_num_instr = 0;
            build_table(filename);
        }
        cout<<"\n";

    }
    catch(int error_code){
        ParseError(error_code);
    };
//***************************************************************************
    cout<<"\n";

    return 0;
};
