SELECT [CompanyCode]
                  ,[InnerCode]
                  ,[SecuCode]
                  ,[ChiName]
                  ,[ChiNameAbbr]
                  FROM [JYDB].[dbo].[SecuMain]
                  where SecuCode like '000002'
                  and SecuMarket in (83,90)
                  and SecuCategory=1;