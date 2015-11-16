
function [] = process_files()
clear all
listfiles = dir('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\*.fvecs');
%disp(listfiles)
path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\';
filename1 = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_BOW4096\\';
filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\query_result4_unmorm';
filename_name = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_data.dat';
filename_name_with_loc = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\Ungulate_data_loc.dat'; 

%[fid,errmsg] = fopen(filename,'r');
  % disp(errmsg);
%fid = fopen(filename,'w+');
%fim = fopen(filename_m,'w+');
n = length(listfiles);
r = randi(n,1,n);
db = r(:,1:n/2);
query = r(:,n/2:n);
loc = 

for i = 1:n
   dlmwrite(filename_m,listfiles(i).name,'-append','delimiter','\n','roffset',1)   
end

for i = 1:n/5
    filename2 = strcat(filename1,listfiles(db(i)).name);
    [pathstr,name,ext] = fileparts(listfiles(db(i)).name);
    newpath = strcat(path,name);
    disp(filename2);
   
    %listfiles(i).name = strcat(path,listfiles(i).name);
    %disp(listfiles(i).name);
    v = fvecs_read(filename2);
    %disp(v)
    [m,n] = size(v);
    if i == 1
        for a = 1:n
            loc = 
            
        end
    
       %  for l = 1:m
            %disp(l);
            %single_v = v(l,:);
        %    for a = 1:n
         %       v(l,a) = abs(v(l,a))^(0.5) * sign(v(l,a));
          %  end
           %v(l,:) = v(l,:)/ norm(v(l,:));
         %end
         
        % rdm = randi(n,1,100)
         %for k = 1:100
          %   if k == 1
           %     v1 = v(:,rdm(k));
            % else
             %   v1 = cat(2,v1,v(:,rdm(k)));
             %end
         %end
         
  
    if i == 1
        dbset = v;
    else
        dbset = cat(2,dbset,v);
    end
    
    %disp(v)
    %figure
    %bar(v)
    %title('Visual word occurrences')
    %xlabel('Visual word index')
    %ylabel('Frequency of occurrence')
    %hist(transpose(v))

                % % for l = 1:m
                    %disp(l);
                    %single_v = v(l,:);
                % %    for a = 1:n
                % %        v(l,a) = abs(v(l,a))^(0.5) * sign(v(l,a));
                % %   end
                % %   v(l,:) = v(l,:)/ norm(v(l,:));
                % % end
    %dlmwrite(newpath,v)
    %fprintf(fid,v);
    %dlmwrite(filename_m,meta,'-append','delimiter',' ','roffset',1)
    %fprintf(fim,meta);
end

kd_tree = vl_kdtreebuild(dbset);

for i= 1:1%n/2
    filename2 = strcat(filename1,listfiles(query(i)).name);
    [pathstr,name,ext] = fileparts(listfiles(query(i)).name);
    newpath = strcat(path,name);
    v = fvecs_read(filename2);
    %disp(v)
    [m,n] = size(v);
    
    %     for l = 1:m
            %disp(l);
            %single_v = v(l,:);
     %       for a = 1:n
      %          v(l,a) = abs(v(l,a))^(0.5) * sign(v(l,a));
       %     end
        %   v(l,:) = v(l,:)/ norm(v(l,:));
         %end
          %rdm = randi(n,1,100)
         %for k = 1:100
          %   if k == 1
           %     v1 = v(:,rdm(k));
           %  else
            %    v1 = cat(2,v1,v(:,rdm(k)));
            % end
         %end
   [index, distance] = vl_kdtreequery(kd_tree, dbset, v, 'NumNeighbors', 10) ;
   disp(size(index));
   disp(size(distance));
   dlmwrite(filename_m,index,'-append','delimiter','\t','roffset',1)
   dlmwrite(filename_m,distance,'-append','delimiter','\t','roffset',1)
   
end

%fclose(fid);
%fclose(fim);