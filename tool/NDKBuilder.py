#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    NDKBuilder --

SYNOPSIS
    NDKBuilder [-h] [-r] -p ProjectRootDir

    -h show help
    -r release mode
    -p project root directory
    -c clean binary files
"""

import os
import sys
import getopt
import subprocess
import shutil

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.path.split(scriptRoot)[0]

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def buildRapid2D(projectRoot, isRelease):
    projectRoot = os.path.realpath(projectRoot)

    if False == os.path.exists(projectRoot) or False == os.path.isdir(projectRoot):
        print "Error: %s is not a directory" %(projectRoot)
        return
    
    jniDir = joinDir(projectRoot, "android", "Rapid2D", "jni")
    if False == os.path.exists(jniDir):
        print "Error: %s is not a Rapid2D project directory" %(projectRoot)
        return

    # check ndk env
    print "====> checking for ndk-build commond\n"
    cmd = subprocess.Popen('ndk-build -v', shell=True, stdout=subprocess.PIPE)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error: ndk-build commond not found!! \
                \n** Please set you Android NDK root to your system Path **"
        return

    # nkd-build
    print "====> start building library\n"
    new_env = os.environ.copy()
    new_env['RAPID2D_ROOT'] = engineRoot # env for Android.mk
    os.chdir(jniDir) # change work dir to jni root
    cmd = subprocess.Popen('ndk-build NDK_DEBUG=%s NDK_MODULE_PATH=%s' \
            %((0 if isRelease else 1), engineRoot), shell=True, env=new_env)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error while building, check error above!"
        return

    # mv so to 'src/main/Rapid2D'
    libsDir = joinDir(projectRoot, "android", "Rapid2D", "libs")
    jniLibsDir = joinDir(projectRoot, "android", "Rapid2D", "src", "main", "Rapid2D")

    print "====> Moveing *.so to jniLibs\n"
    if os.path.exists(jniLibsDir):
        shutil.rmtree(jniLibsDir)
    shutil.move(libsDir, jniLibsDir)

    print "====> Copying src,res to Android asserts\n"
    androidAssertsDir = joinDir(projectRoot, "android", "app", "src", "main", "assets")
    resDir = joinDir(projectRoot, "common", "res")
    resTargetDir = joinDir(androidAssertsDir, "res")
    if os.path.exists(resTargetDir):
        shutil.rmtree(resTargetDir)
    shutil.copytree(resDir, resTargetDir)

    luaDir = joinDir(projectRoot, "common", "lua")
    luaTargetDir = joinDir(androidAssertsDir, "lua")
    if os.path.exists(luaTargetDir):
        shutil.rmtree(luaTargetDir)
    shutil.copytree(luaDir, luaTargetDir)

    print "====> Build Done\n"

def cleanRapid2D(projectRoot):
    jniDir = joinDir(projectRoot, "android", "Rapid2D", "jni")
    if False == os.path.exists(jniDir):
        print "Error: %s is not a Rapid2D project directory" %(projectRoot)
        return

    print "====> start cleaning\n"
    new_env = os.environ.copy()
    new_env['RAPID2D_ROOT'] = engineRoot # env for Android.mk
    os.chdir(jniDir) # change work dir to jni root
    cmd = subprocess.Popen('ndk-build clean NDK_MODULE_PATH=%s' \
            %(engineRoot), shell=True, env=new_env)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error while cleaning, check error above!"
        return

    print "====> clean done\n"

if __name__ == "__main__":
    isRelease = False
    projectRoot = None
    isClean = False

    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hrp:c")
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-r":
            isRelease = True
        if o == "-p":
            projectRoot = a
        if o == "-c":
            isClean = True

    if projectRoot == None:
        print "Error: Required parameter '-p'"
        sys.exit(0)

    # run build
    if isClean:
        cleanRapid2D(projectRoot)
    else:
        buildRapid2D(projectRoot, isRelease)
