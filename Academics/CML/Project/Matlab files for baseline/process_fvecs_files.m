
function [] = process_files()
clear all
listfiles = dir('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_BOW4096\\*.fvecs');
%disp(listfiles)
path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Extracted_BOW4096\\';
filename = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\fvecs_desc_k2000';
%filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\siftgeo_meta_siftgeo';
 %[fid,errmsg] = fopen(filename,'r');
  % disp(errmsg);
%fid = fopen(filename,'w+');
%fim = fopen(filename_m,'w+');
for i = 1:length(listfiles)
    newpath = strcat(path,listfiles(i).name);
    %listfiles(i).name = strcat(path,listfiles(i).name);
    disp(listfiles(i).name);
    disp(newpath);
    v = fvecs_read(newpath);
    dlmwrite(filename,v,'-append','delimiter',' ','roffset',1)
    %fprintf(fid,v);
    %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
    %fprintf(fim,meta);
end
%fclose(fid);
%fclose(fim);