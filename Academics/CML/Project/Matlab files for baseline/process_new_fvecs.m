
function [] = process_files()
clear all
listfiles = dir('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_BOW4096\\*.fvecs');
%disp(listfiles)
path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Extracted_tab_BOW4096\\';
filename1 = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_BOW4096\\';
%filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\siftgeo_meta_siftgeo';
 %[fid,errmsg] = fopen(filename,'r');
  % disp(errmsg);
%fid = fopen(filename,'w+');
%fim = fopen(filename_m,'w+');

for i = 1:length(listfiles)
    filename2 = strcat(filename1,listfiles(i).name);
    [pathstr,name,ext] = fileparts(listfiles(i).name);
    newpath = strcat(path,name);
    disp(filename2);
   
    %listfiles(i).name = strcat(path,listfiles(i).name);
    %disp(listfiles(i).name);
    v = fvecs_read(filename2);
    
    
    dlmwrite(newpath,v,'delimiter','\t');
    %fprintf(fid,v);
    %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
    %fprintf(fim,meta);
end
%fclose(fid);
%fclose(fim);