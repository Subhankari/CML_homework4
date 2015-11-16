function [] = process_files()
clear all

infile = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\merged_data.dat';
all_data = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\all_data.dat';
result_data = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\result_data.dat';
train_names = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\train_name1.dat';
test_names = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\test_name1.dat';

data = dlmread(infile);

[rdim,cdim] = size(data);

for i = 1:int32(ceil(rdim/100))
    r = randi(50,1,50);
    [rdim1,rdim2] = size(r);
    for j = 1:rdim2
        idxval = (i - 1) * 100 + r(1,j);
        idxval1 = (i - 1) * 100 + r(1,j) + 50;
        %idx = (i - 1) * rdim2 + j;
        if i == 1 
            if j == 1
                train = data(idxval,:);
                test = data(idxval1,:);  
            else
                train = cat(1,train,data(idxval,:));
                test = cat(1,test,data(idxval1,:));
            end
            dlmwrite(train_names,idxval,'-append','delimiter','','roffset',0);
            dlmwrite(test_names,idxval1,'-append','delimiter','','roffset',0);
            val = strcat(int2str(idxval),'.jpg');
            dlmwrite(all_data,val,'-append','delimiter','');
            val = strcat(int2str(idxval1),'.jpg');
            dlmwrite(all_data,val,'-append','delimiter','');
          
        else
            idxval = (i - 1) * 100 + r(1,j);
            idxval1 = (i - 1) * 100 + r(1,j) + 50;
            idx = (i - 1) * rdim2 + j;
            train = cat(1,train,data(idxval,:));
            test = cat(1,test,data(idxval1,:));
            dlmwrite(train_names,idxval,'-append','delimiter','','roffset',0);
            dlmwrite(test_names,idxval1,'-append','delimiter','','roffset',0);
            val = strcat(int2str(idxval),'.jpg');
            dlmwrite(all_data,val,'-append','delimiter','');
            val = strcat(int2str(idxval1),'.jpg');
            dlmwrite(all_data,val,'-append','delimiter','');
        end
    end
end
store_train1 = dlmread(train_names,'');
store_test1 = dlmread(test_names,'');
disp('load');
[m,n] = size(train);
[m1,n1] = size(test);

for i = 1:m    
    for j = 1:n
        train(i,j) = abs(train(i,j))^(0.5) * sign(train(i,j));
    end
end
train = train/norm(train);
for i = 1:m1    
    for j = 1:n1
        test(i,j) = abs(test(i,j))^(0.5) * sign(test(i,j));
    end
end
test = test/norm(test);
disp('norm');
train = transpose(train);
test = transpose(test);

kd_tree = vl_kdtreebuild(train);

[index, distance] = vl_kdtreequery(kd_tree, train, test, 'NumNeighbors', 3,'MaxComparisons', 15) ;
IDX = knnsearch(transpose(train),transpose(test),'k',10);
disp(IDX);
disp(index);
[indrow,indcol] = size(index);
[m1,n1] = size(test);

disp('search over');

for i = 1:n1
    imgname = strcat(int2str(store_test1(i)),'.jpg');
    result = imgname;
    for p = 1:indrow
        inter1 = index(p,i);
        inter1 = strcat(int2str(store_train1(inter1)),'.jpg');
        p = p - 1;
        inter2 = cat(2,' ',int2str(p),' ',inter1);
        result = cat(2,result,inter2);
    end
    disp(result);
    dlmwrite(result_data,result,'-append','delimiter','','roffset',0);
end



