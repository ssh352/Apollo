%% Commodity Inventory Strategy
%   ANK Lou
%   2018/05/25
%   Version info:
%       1. Read info of xlsx named '¿â´æ', download data respectively from
%       wind according to it.
%       2. Unify all unit tags of commodities to unit 1.
%       3. Lag foreign exchanges datetime.

clear
clc

xlsxPath  = 'F:\k¿â´æ²ßÂÔ\';
xlsxName  = '¿â´æ.xlsx';
sheetName = 'Codes';

[~, Inventory, ~] = xlsread([xlsxPath,xlsxName], sheetName);

%%
w = windmatlab;
for i = 1:size(Inventory,1)
    [data{i,1},data{i,2},data{i,3},data{i,4},data{i,5},data{i,6}] = w.edb(Inventory{i,8},'1991-01-01','2018-05-25','Fill=Previous');
    
    % Pure data
    Inventory{i,9} = [data{i,4}, data{i,1}];
    
    % Cell for saving
    Inventory{i,10}{1,1}   = 'ÈÕÆÚ';
    Inventory{i,10}(1,2:6) = Inventory(i,3:7);
    Ni = size(data{i,4},1);
    for t = 1:Ni
        Inventory{i,10}{t+1,1} = datestr(data{i,4}(t),26);
    end
    Nj = size(data{i,1},2);
    Inventory{i,10}(2:Ni+1,2:Nj+1) = num2cell(data{i,1});
    
    % Unify unit tag
    Inventory{i,11}(:,1) = data{i,4};
    for j = 1:Nj
        tmp.isnan = find(~isnan(data{i,1}(:,j)));
        tmp.srl   = data{i,1}(tmp.isnan,j);
        if i <= 8 && j > 1
            if tmp.isnan(1) == 1
                tmp.isnan(1) = [];
                tmp.srl(1)   = [];
            end
            Inventory{i,11}(tmp.isnan-1,j+1) = [0;tmp.srl(2:end)./tmp.srl(1:end-1)-1];
        else
            Inventory{i,11}(tmp.isnan,j+1)   = [0;tmp.srl(2:end)./tmp.srl(1:end-1)-1];
        end
        clear tmp
    end
    for t = 1:Ni
        tmp.isnan = find(isnan(Inventory{i,11}(t,:)));
        Inventory{i,11}(t,tmp.isnan) = zeros(1,size(tmp.isnan,2));
        tmp.isinf = find(isinf(Inventory{i,11}(t,:)));
        Inventory{i,11}(t,tmp.isinf) = zeros(1,size(tmp.isinf,2));
        
        tmp.isn0 = find(Inventory{i,11}(t,2:Nj+1)~=0);
        n        = size(tmp.isn0,1);
        if n ~= 0
            tmp.tsrl(t,1) = sum(Inventory{i,11}(t,2:Nj+1),'omitnan')/n;
        else
            tmp.tsrl(t,1) = sum(Inventory{i,11}(t,2:Nj+1),'omitnan');
        end
    end
    tmp.tsrl(:,2)   = 1 + cumsum(tmp.tsrl(:,1));
    Inventory{i,11} = [Inventory{i,11}, tmp.tsrl];
    
    xlswrite([xlsxPath,Inventory{i,1}],Inventory{i,10},Inventory{i,2})
end

%%
save([xlsxPath,'Commodity_Inventory.mat'],'Inventory')

%%
clf

for i = 1:size(Inventory,1)
    subplot(8,4,i)
    plot(Inventory{i,11}(:,1),Inventory{i,11}(:,end))
    datetick('x','yyyy/mm/dd','keepticks')
    legend(Inventory{i,2},7,'Location','NorthWest')
end