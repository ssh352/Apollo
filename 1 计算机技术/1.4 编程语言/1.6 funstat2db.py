#!/usr/bin/env python
# -*- coding: utf-8 -*-

###################
# Author: x00321237
# Date: 2017-03-30
# Description: Download repo and the numbers of testcases
#########################################################

import os
import MySQLdb
from git import Repo

DB_SERVER = "100.106.59.159"
DB_NAME = "smoketest"
DB_ROOT = "root"
DB_PWD = ""
TB_NAME = "testcase"

FN_TEMPEST = "http://cloudos-review.huawei.com/cloudos-tests/fn-tempest-plugin"
FI_TEMPEST = "http://cloudos-review.huawei.com/cloudos-tests/fi-tempest-plugin"
FK_TEMPEST = "http://cloudos-review.huawei.com/cloudos-tests/fk-tempest-plugin"

def repo_clone(url, path):

    Repo.clone_from(url, path)
    if os.path.exists(path):
        return True
    else:
        return False

def stat_case(project, path):
    num = 0
    for (parent, dirs, files) in os.walk(path):
        for filename in files:
            pathname = []
            print "filename=%s" % filename
            pathname += [os.path.join(parent, filename)]
            print pathname[0]
            with open (pathname[0], "r+") as f:
                for line in f.readlines():
                    if line.strip().startswith("def"):
                        lists = line.split()
                        if lists[0] == "def" and lists[1].startswith("test"):
                            print line
                            num += 1
    print "num=%-3d" % num
    print project.split("/")[-1]
    write_db(project.split("/")[-1], num)
    return num

def write_db(projectname, casesum):
    try:
        con = MySQLdb.connect(host=DB_SERVER, user=DB_ROOT, passwd=DB_PWD, db=DB_NAME)
        cur = con.cursor()
        cur.execute("INSERT st_project VALUES('%s', curdate(), %d);" % (projectname, int(casesum)))
        con.commit()
        cur.close()
    finally:
        if con:
            con.close()

# fn-tempest-plugin
fnpath = FN_TEMPEST.split("/")[-1]
os.system('rm -rf %s' % fnpath)
repo_clone(FN_TEMPEST, fnpath)
abs_path = os.path.abspath(fnpath)
test_path = abs_path+"/fn_tempest_plugin/tests"
stat_case(FN_TEMPEST, test_path)

# fi-tempest-plugin
fipath = FI_TEMPEST.split("/")[-1]
os.system('rm -rf %s' % fipath)
repo_clone(FI_TEMPEST, fipath)
abs_path_fi = os.path.abspath(fipath)
test_path_fi = abs_path_fi+"/fi_tempest_plugin/tests"
stat_case(FI_TEMPEST, test_path_fi)

# fk-tempest-plugin
fkpath = FK_TEMPEST.split("/")[-1]
os.system('rm -rf %s' % fkpath)
repo_clone(FK_TEMPEST, fkpath)
abs_path_fk = os.path.abspath(fkpath)
test_path_fk = abs_path_fk+"/fk_tempest_plugin/tests"
stat_case(FK_TEMPEST, test_path_fk)
                                                                                     1,1           Top
