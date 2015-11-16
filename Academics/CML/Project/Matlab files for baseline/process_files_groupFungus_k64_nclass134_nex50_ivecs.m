function [] = process_files()
clear all
listfiles = dir('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass10_nex10\\example_data\\*.ivecs');
%disp(listfiles)
path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass10_nex10\\example_data\\';
filename = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass10_nex10\\extracted_data\\';
filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\flickr60K_meta_100';
 %[fid,errmsg] = fopen(filename,'r');
  % disp(errmsg);
%fid = fopen(filename,'w+');
%fim = fopen(filename_m,'w+');
n = length(listfiles);
for i = 1:n
    [pathstr,name,ext] = fileparts(listfiles(i).name)
    newpath = strcat(filename,name);
    disp(newpath);
    %listfiles(i).name = strcat(path,listfiles(i).name);
    npath = strcat(path,listfiles(i).name);
    disp(npath);
    v = ivecs_read(npath);
    dlmwrite(newpath,v,'-append','delimiter','\t','roffset',1)
    %fprintf(fid,v);
    %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
    %fprintf(fim,meta);
end
%fclose(fid);
%fclose(fim);