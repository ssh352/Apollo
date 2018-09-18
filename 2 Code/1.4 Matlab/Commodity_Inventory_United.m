%% Commodity Inventory Strategy
%   ANK Lou
%   2018/05/25
%   Version info:
%       1. Read info of xlsx named '库存', download data respectively from
%       wind according to it.
%       2. Unify all unit tags of commodities to unit 1.
%       3. Lag foreign exchanges datetime.

clear
clc

xlsxPath  = 'F:\Matlab\data\';
xlsxName  = '库存.xlsx';
sheetName = 'Codes';

[~, Inventory, ~] = xlsread([xlsxPath,xlsxName], sheetName);

load([xlsxPath,'Commodity_Inventory.mat'])

%%
for i = 1:size(Inventory,1)
    Ni = size(Inventory{i,9},1);
    Nj = size(Inventory{i,9},2);
    
    Inventory{i,12}(:,1) = Inventory{i,9}(:,1);
    
    if i == 1
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2:3),Inventory{i,9}(:,4)*0.9071847] ,2,'omitnan' );
    elseif i >= 2 && i <= 6 || i == 9 || i == 12 || i == 32
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2:Nj)] ,2,'omitnan' );
    elseif i == 7 || i == 8
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2), Inventory{i,9}(:,3)*31.1034768*1e-3] ,2,'omitnan' );
    elseif i == 10 || i == 11 || i >= 14 && i <= 16 || i == 30 || i >= 22 && i <= 24
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2:Nj)*1e5] ,2,'omitnan' );
    elseif i == 13
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2)*1e5,Inventory{i,9}(:,3)] ,2,'omitnan' );
    elseif i == 17
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2)*10, Inventory{i,9}(:,3:Nj)*1e5] ,2,'omitnan' );
    elseif i == 18
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2)*10] ,2,'omitnan' );
    elseif i >= 19 && i <= 21
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2)*10, Inventory{i,9}(:,3)] ,2,'omitnan' );
    elseif i == 25 || i == 26 || i == 28 || i == 31
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2:Nj)*5] ,2,'omitnan' );
    elseif i == 27
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2)*5, Inventory{i,9}(:,Nj)] ,2,'omitnan' );
    elseif i == 29
        Inventory{i,12}(:,2) = sum( [Inventory{i,9}(:,2:Nj-1)*1e5, Inventory{i,9}(:,Nj)] ,2,'omitnan' );
    end
    Inventory{i,12} = Inventory{i,12}(Inventory{i,12}(:,2)~=0,:);
    Inventory{i,12}(:,3) = Inventory{i,12}(:,2)/Inventory{i,12}(1,2);
    
    for t = linspace(Ni, 1, Ni)
        if Inventory{i,9}(t,1) - 368 >= Inventory{i,9}(1,1)
            % 同比
            idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 365);
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 364);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 366);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 363);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 367);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 362);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 368);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 361);
            end
            tmp.ltb = Inventory{i,9}(idate,:);
            if ~isempty(tmp.ltb)
                itb = find(~isnan(tmp.ltb(1,2:end)))+1;
                Inventory{i,13}(t,1) = Inventory{i,9}(t,1);
                Inventory{i,13}(t,2) = sum(Inventory{i,9}(t,itb),2)/sum(tmp.ltb(itb),2);
            end
            clear idate
            
            % 环比
            idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 30);
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 29);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 31);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 28);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 32);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 27);
            end
            if isempty(idate)
                idate = find(Inventory{i,9}(:,1) == Inventory{i,9}(t,1) - 33);
            end
            tmp.lhb = Inventory{i,9}(idate,:);
            if ~isempty(tmp.lhb)
                ihb = find(~isnan(tmp.lhb(1,2:end)))+1;
                Inventory{i,13}(t,3) = sum(Inventory{i,9}(t,ihb),2)/sum(tmp.lhb(ihb),2);
            end
            clear idate
        end
        if isinf(Inventory{i,13}(t,2)) || size(Inventory{i,13},2) == 3 && isinf(Inventory{i,13}(t,3))
            Inventory{i,13}(t,1:3) = [0,0,0];
        end
    end
    for t = linspace(Ni, 1, Ni)
        if Inventory{i,13}(t,2) > mean(Inventory{i,13}(:,2)) + 3*std(Inventory{i,13}(:,2))
            Inventory{i,13}(t,1:3) = [0,0,0];
        end
    end
    Inventory{i,13} = Inventory{i,13}(Inventory{i,13}(:,1)~=0,:);
    
    for t = 1:size(Inventory{i,13},1)
        Inventory{i,14}{t,1} = datestr(Inventory{i,13}(t,1),26);
    end
    Inventory{i,14} = [Inventory{i,14},num2cell(Inventory{i,13}(:,2:end))];
    tmp.title = {'date','同比','环比'};
    Inventory{i,14} = [tmp.title;Inventory{i,14}];
    
    xlswrite([xlsxPath,'同比与环比\',Inventory{i,1}],Inventory{i,14},Inventory{i,2})
    
end

%%
clf

for i = 1:size(Inventory,1)
    subplot(8,4,i)
    plot(Inventory{i,13}(Inventory{i,13}(:,2)~=0,1),Inventory{i,13}(Inventory{i,13}(:,2)~=0,2))
    datetick('x','yyyy/mm/dd','keepticks')
    legend(Inventory{i,2},7,'Location','NorthWest')
end

%%
% i = 13;
% clf
% plot(Inventory{i,12}(:,1),Inventory{i,12}(:,3))
% datetick('x','yyyy/mm/dd','keepticks')
% legend(Inventory{i,2},7,'Location','NorthWest')