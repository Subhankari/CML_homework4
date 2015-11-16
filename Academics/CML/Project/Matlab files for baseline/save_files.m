function [] = process_files()
clear all

first_files = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\groupFungus_k64_nclass134_nex50_Xtrain.fvecs';
%path = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\';
filename_m = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\result2';
second_files = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\groupFungus_k64_nclass134_nex50_Xtest.fvecs';
all_data = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\all_data.dat';
result_data = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\groupFungus_k64_nclass134_nex50\\example_data\\result_data.dat';
v1 = fvecs_read(first_files);
%v2 = fvecs_read(second_files);

v1 = transpose(v1);
[m,n] = size(v1);

for l = 1:m  
    %disp(l);
    %single_v = v(l,:);
    for a = 1:n
        v1(l,a) = abs(v1(l,a))^(0.5) * sign(v1(l,a));
    end
end
 v1 = v1/norm(v1);
  bar(v1(1,:));
 title('Visual word occurrences')
 xlabel('Visual word index')
 ylabel('Frequency of occurrence')

for l = 1:m   
    if l == 1
        v2 = v1(l,:);
    else if mod(l,50) == 1
            %v2 = cat(2,v2,v1(:,l));
            %v1(:,l) = [];
        end
    end
    data = strcat(int2str(l),'.jpg');
    disp(data);
    dlmwrite(all_data,data,'-append','delimiter','');
 end
 bar(v1(1,:));
 title('Visual word occurrences')
 xlabel('Visual word index')
 ylabel('Frequency of occurrence')
 
 [m1,n1] = size(v2);
 disp(size(v2));   
 %for o = 1:n1    
    %disp(l);
    %single_v = v(l,:);
  %  for b = 1:m1
   %     v2(b,o) = abs(v2(b,o))^(0.5) * sign(v2(b,o));
   % end
   % v2(:,o) = v2(:,o)/ norm(v2(:,o));
   % val = n + o;
   % disp(val);
   % data = strcat(int2str(val),'.jpg');
   % dlmwrite(all_data,data,'-append','delimiter','');
 %end

v1 = transpose(v1);
v2= transpose(v2);

kd_tree = vl_kdtreebuild(v1);

[index, distance] = vl_kdtreequery(kd_tree, v1, v2, 'NumNeighbors', 10) ;
IDX = knnsearch(transpose(v1),transpose(v2),'k',10);
disp(IDX);
disp(index);
disp(size(index));
[indrow,indcol] = size(index);
[m1,n1] = size(v2);
for o1 = 1:n1
    inter = ((o1 - 1) * 50 ) + 1 ;
    imgname = strcat(int2str(inter),'.jpg');
    result = imgname;
    for p = 1:indrow
        inter1 = index(p,o1);
        inter1 = strcat(int2str(inter1),'.jpg');
        p = p - 1;
        inter2 = cat(2,' ',int2str(p),' ',inter1);
        result = cat(2,result,inter2);
    end
    disp(result);
    dlmwrite(result_data,result,'-append','delimiter','','roffset',0);
end

disp(size(distance));

dlmwrite(filename_m,index,'-append','delimiter','\t','roffset',1)
dlmwrite(filename_m,distance,'-append','delimiter','\t','roffset',1)



