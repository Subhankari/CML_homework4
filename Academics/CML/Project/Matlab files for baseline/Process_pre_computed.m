
function [] = process_files()
clear all
listdir = dir('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\siftgeo1M');
%disp(listfiles)
path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\siftgeo1M\\';
path1 = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\sift_normal_pl\\';
filename = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\unnormalized\\';
filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\flickr60K_meta_100';
k = 0; 
%[fid,errmsg] = fopen(filename,'r');
  % disp(errmsg);
%fid = fopen(filename,'w+');
%fim = fopen(filename_m,'w+');
for i = 1:length(listdir)
    newpath = strcat(path,listdir(i).name);
    newpath = strcat(newpath,'\\*.siftgeo');
    dot = '.';
    match = any(strfind(listdir(i).name,dot));
    disp(match);
    space = strcmp(listdir(i).name,'');
    tmppath = strcat(path,listdir(i).name);
    tmpwritepath = strcat(filename,listdir(i).name);
    if ~(match || space)
        disp('newpath');
        disp(newpath);
        listfiles = dir(newpath);
        if length(listfiles) < 20
            for j = 1:length(listfiles)
                newpath1 = strcat(tmppath,'\\');
                [pathstr,name,ext] = fileparts(listfiles(j).name)
                newpath1 = strcat(newpath1,listfiles(j).name);
                tmpwritepath1 = strcat(tmpwritepath,'_');
                tmpwritepath1 = strcat(tmpwritepath1,name);

                %listfiles(i).name = strcat(path,listfiles(i).name);
                disp(listfiles(j).name);
                disp(newpath1);
                [v,meta] = siftgeo_read(newpath1);
                [m,n] = size(v);
                %for l = 1:m
                    %disp(l);
                    %single_v = v(l,:);
                 % %  for a = 1:n
                  % %     v(l,a) = abs(v(l,a))^(0.5) * sign(v(l,a));
                  % %  end
                    % % v(l,:) = v(l,:)/ norm(v(l,:));
                %end

                %dlmwrite(filename,newpath1,'-append','delimiter',' ','roffset',1)
                %dlmwrite(filename,m,'-append','delimiter',' ','roffset',1)
                 disp(tmpwritepath1);
                 dlmwrite(tmpwritepath1,v)
                %fprintf(fid,v);
                %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
                %fprintf(fim,meta);
            end
        else
            for j = 1:20
                r = randi(length(listfiles),20,1);
                newpath1 = strcat(tmppath,'\\');
                [pathstr,name,ext] = fileparts(listfiles(r(j)).name);
                newpath1 = strcat(newpath1,listfiles(r(j)).name);
                tmpwritepath1 = strcat(tmpwritepath,'_');
                tmpwritepath1 = strcat(tmpwritepath1,name);

                %listfiles(i).name = strcat(path,listfiles(i).name);
                disp(listfiles(r(j)).name);
                disp(newpath1);
                [v,meta] = siftgeo_read(newpath1);
                [m,n] = size(v);
                
                
                % % for l = 1:m
                    %disp(l);
                    %single_v = v(l,:);
                % %    for a = 1:n
                % %        v(l,a) = abs(v(l,a))^(0.5) * sign(v(l,a));
                % %   end
                % %   v(l,:) = v(l,:)/ norm(v(l,:));
                % % end

                %dlmwrite(filename,newpath1,'-append','delimiter',' ','roffset',1)
                %dlmwrite(filename,m,'-append','delimiter',' ','roffset',1)
                disp(tmpwritepath1);
                dlmwrite(tmpwritepath1,v)
                %fprintf(fid,v);
                %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
                %fprintf(fim,meta);
            end
            
        end
    end
      
end
%fclose(fid);
%fclose(fim);