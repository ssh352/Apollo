w=windmatlab; 
 
tradeday=w.tdays('2010-01-01','2018-04-25');
frame_cell=cell(length(tradeday),1);
for i=1:1:length(tradeday)
  fun_str_temp=strcat('date=',tradeday(i));
  fun_str=strcat(fun_str_temp,';sectorid=1000008491000000');
  zz_frame=w.wset('sectorconstituent',fun_str);
  frame_cell{i}=zz_frame(:,2);
end
 
temp_date=frame_cell{1};
for i=1:1:length(frame_cell)
    temp_date=unique(union(temp_date,frame_cell{i}));
end
 
zz_code=temp_date;
return_max=zeros(length(tradeday),length(zz_code));
turnover_max=zeros(length(tradeday),length(zz_code));
turnovercap_float=zeros(length(tradeday),length(zz_code));
for i=915:1:length(zz_code)
    return_max(:,i)=w.wsd(zz_code(i),'pct_chg','2010-01-01','2018-04-25')/100;
    turnover_max(:,i)=w.wsd(zz_code(i),'turn','2010-01-01','2018-04-25')/100;
    turnovercap_float(:,i)=w.wsd(zz_code(i),'mkt_cap_float','2010-01-01','2018-04-25','unit=1','currencyType=');
end
zz_re=w.wsd('399905.SZ','pct_chg','2010-01-01','2018-04-25');
zz_re=zz_re/100;
%%
exchange_return_list_neutral=zeros(length(tradeday),5);
per_day=100;
position_num=0:0.2:1;
change_day=per_day:20:length(tradeday);
change_day=unique([change_day,length(tradeday)]);
for i=1:1:length(change_day)-1
    compare_max=ones(length(zz_code),3);
    compare_max(:,1)=1:1:length(zz_code);
    compare_max(:,2)=sum(return_max((change_day(i)-per_day+1):change_day(i),:));
    compare_max(:,3)=sum(log(turnovercap_float((change_day(i)-per_day+1):change_day(i),:)));
    compare_max=compare_max(find(isnan(compare_max(:,2))~=1),:);
    compare_max=compare_max(find(abs(compare_max(:,3))~=Inf),:);
    compare_max=sortrows(compare_max,2);
    len=length(compare_max(1:end,:));
    compare_max=compare_max((1+ceil(len*0.02)):(len-ceil(len*0.02)),:);
    xx=[ones(size(compare_max(:,3))),zscore(compare_max(:,3))];
    yy=zscore(compare_max(:,2));
    [b,bint,r,rint,stats]=regress(yy,xx);
    
    final_max=ones(length(compare_max(:,1)),3);
    final_max(:,1)=compare_max(:,1);
    final_max(:,2)=r;
    final_max(:,3)=compare_max(:,3);
    final_max=sortrows(final_max,2);
    for ii=1:1:5
        max_index=ceil(length(final_max(:,1))*position_num(ii))+1:ceil(length(final_max(:,1))*position_num(ii+1));
        select_index=final_max(max_index,1);
        %select_index=compare_max(end-position_num+1:end,1);
        select_max=return_max((change_day(i)+1):change_day(i+1),select_index);
        [aa,bb]=find(isnan(select_max)==1);
        select_max(aa,bb)=0;
        exchange_return_list_neutral((change_day(i)+1):change_day(i+1),ii)=sum(select_max,2)/length(max_index);
    end
end
for i=1:1:5
    plot(cumsum(exchange_return_list_neutral(per_day:end,i)-zz_re(per_day:end)),'Color',[(0+0.1/i)^2 0.8-(0.1*i) 0.7/i^2])
    hold on
end
plot(cumsum(exchange_return_list_neutral(per_day:end)-zz_re(per_day:end)),'b')
%%
exchange_return_list_neutral=zeros(length(tradeday),1);
per_day=50;
position_num=50;
change_day=per_day:20:length(tradeday);
change_day=unique([change_day,length(tradeday)]);
for i=1:1:length(change_day)-1
    compare_max=ones(length(zz_code),3);
    compare_max(:,1)=1:1:length(zz_code);
    compare_max(:,2)=sum(return_max((change_day(i)-per_day+1):change_day(i),:));
    compare_max(:,3)=sum(log(turnovercap_float((change_day(i)-per_day+1):change_day(i),:)));
    compare_max=compare_max(find(isnan(compare_max(:,2))~=1),:);
    compare_max=compare_max(find(abs(compare_max(:,3))~=Inf),:);
    compare_max=sortrows(compare_max,2);
    len=length(compare_max(1:end,:));
    compare_max=compare_max((1+ceil(len*0.02)):(len-ceil(len*0.02)),:);
    xx=[ones(size(compare_max(:,3))),zscore(compare_max(:,3))];
    yy=zscore(compare_max(:,2));
    [b,bint,r,rint,stats]=regress(yy,xx);
    
    final_max=ones(length(compare_max(:,1)),3);
    final_max(:,1)=compare_max(:,1);
    final_max(:,2)=r;
    final_max(:,3)=compare_max(:,3);
    final_max=sortrows(final_max,2);
    max_index=1:position_num;
    select_index=final_max(max_index,1);
    %select_index=compare_max(end-position_num+1:end,1);
    select_max=return_max((change_day(i)+1):change_day(i+1),select_index);
    [aa,bb]=find(isnan(select_max)==1);
    select_max(aa,bb)=0;
    exchange_return_list_neutral((change_day(i)+1):change_day(i+1))=sum(select_max,2)/length(max_index);
end
plot(cumsum(exchange_return_list_neutral(per_day:end)-zz_re(per_day:end)),'b')
 
%%=========================================?¨¤?¨¹??=========================================%%
per_day=20;
position_num=0:0.2:1;
running_return_list=zeros(length(tradeday),5);
change_day=per_day:20:length(tradeday);
change_day=unique([change_day,length(tradeday)]);
for i=1:1:length(change_day)-1
    compare_max=ones(length(zz_code),3);
    compare_max(:,1)=1:1:length(zz_code);
    %========================????¨®???¨º?1??¦Ìlag¨°?¨¬¨¬==========================%
    turnover_max_temp=turnover_max((change_day(i)-per_day+1):(change_day(i)-1),:);
    return_max_temp=return_max((change_day(i)-per_day+2):change_day(i),:);
    for j=1:1:length(turnover_max_temp(1,:))
        compare_max(j,2)=corr(zscore(turnover_max_temp(:,j)),zscore(return_max_temp(:,j)),'type','Spearman');
        %compare_max(j,2)=corr(zscore(turnover_max_temp(:,j)),zscore(return_max_temp(:,j)));
    end
    compare_max(:,3)=log(turnovercap_float(change_day(i),:));
    compare_max=compare_max(find(isnan(compare_max(:,2))~=1),:);
    compare_max=compare_max(find(abs(compare_max(:,3))~=Inf),:);
 
    
    compare_max=sortrows(compare_max,2);
    %len=length(compare_max(1:end,:));
    %compare_max=compare_max((1+ceil(len*0.02)):(len-ceil(len*0.02)),:);
    %xx=[ones(size(compare_max(:,3))),zscore(compare_max(:,3))];
    %yy=zscore(compare_max(:,2));
    %[b,bint,r,rint,stats]=regress(yy,xx);
    
    final_max=ones(length(compare_max(:,1)),3);
    final_max(:,1)=compare_max(:,1);
    final_max(:,2)=compare_max(:,2);
    final_max(:,3)=compare_max(:,3);
    final_max=sortrows(final_max,2);
    for ii=1:1:5
        max_index=ceil(length(final_max(:,1))*position_num(ii))+1:ceil(length(final_max(:,1))*position_num(ii+1));
        select_index=final_max(max_index,1);
        %select_index=compare_max(end-position_num+1:end,1);
        select_max=return_max((change_day(i)+1):change_day(i+1),select_index);
        [aa,bb]=find(isnan(select_max)==1);
        select_max(aa,bb)=0;
        running_return_list((change_day(i)+1):change_day(i+1),ii)=sum(select_max,2)/length(max_index);
    end
end
for i=1:1:5
    plot(cumsum(running_return_list(per_day:end,i)-zz_re(per_day:end)),'Color',[(0+0.1/i)^2 0.8-(0.1*i) 0.7/i^2])
    hold on
end
%%
per_day=20;
position_num=0:0.2:1;
running_return_list=zeros(length(tradeday),1);
change_day=per_day:20:length(tradeday);
change_day=unique([change_day,length(tradeday)]);
% for i=1:1:length(change_day)-1
%     compare_max=ones(length(zz_code),3);
%     compare_max(:,1)=1:1:length(zz_code);
%     %========================????¨®???¨º?1??¦Ìlag¨°?¨¬¨¬==========================%
%     turnover_max_temp=turnover_max((change_day(i)-per_day+1):(change_day(i)-
