#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    LuaCompiler --

SYNOPSIS
    LuaCompiler [-h] -p projectDir -k key

    -h show help
    -p projectDir: Rapid2D proejct Root directory
    -k xxtea encrypt key
"""

import os
import sys
import getopt
import subprocess
import shutil
import re
import platform
import hashlib
import struct

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.path.split(scriptRoot)[0]

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def compileLuaFiles(projectDir, key):
    luacPath = None
    sysstr = platform.system()
    sysModel = None
    if(sysstr =="Windows"):
        luacPath = joinDir(scriptRoot, "win32", "luac.exe")
        from win32 import xxtea
    elif(sysstr == "Linux"):
        print "Liunux Support is coming sooooon"
        sys.exit(-1)
    elif(sysstr == "Darwin"):
        luacPath = joinDir(scriptRoot, "mac", "luac")
        from mac import xxtea
    else:
        print "Unsupport OS!"
        sys.exit(-1)

    luaDir = joinDir(projectDir, "common", "lua")
    resDir = joinDir(projectDir, "common", "res")

    fileInfo = {}
    for item in os.walk(luaDir):
        for name in item[2]:
            if name[-3:] != "lua":
                continue
            fullPath = joinDir(item[0], name)
            modelPath = fullPath[(len(luaDir) + 1):-4]
            # modelName use in lua require
            if os.sep == '\\':
                modelName = re.sub('\\\\', '.', modelPath)
            else:
                modelName = re.sub('/', '.', modelPath)
            if len(modelName) >= 128:
                print "modelName: %s is to long, please shortcut your lua source path" %(modelName)
                sys.exit(-1)

            # 1. call luac to complie lua source
            outPath = fullPath[:-3] + "out"
            cmd = subprocess.Popen('%s -o %s %s' %(luacPath, outPath, fullPath),
                    shell=True, stdout=subprocess.PIPE)
            cmd.wait()

            if os.path.exists(outPath) == False:
                print "Fail to compile:%s" %(modelPath)
                sys.exit(-1)

            #  2. xxtea encrypt
            fi = open(outPath, "rb")
            inBuffer = fi.read()
            fi.close()
            os.remove(outPath)

            outBuffer = xxtea.encrypt(inBuffer, key)
            fileInfo[modelName] = {
                    'size':len(outBuffer),
                    'buffer':outBuffer
                    }

    fileCounts = len(fileInfo)
    # write to game.rd
    fo = open(joinDir(scriptRoot, "game.rd"), "wb")
    
    # add rapid2d identifier
    identifier = "Rapid2D"
    fo.write(identifier)
    # version number
    byte = struct.pack('I', 1)
    fo.write(byte)
    
    # file counts
    byte = struct.pack('I', fileCounts)
    fo.write(byte)
    # headers
    curOffet = fo.tell() + fileCounts * struct.calcsize('@128sII')
    for modelName in fileInfo:
        # 4 bytes alignment
        # struct {
        #    char modelName[128];
        #    uint32 size;
        #    uint32 offset;
        # }
        byte = struct.pack('@128sII', modelName, fileInfo[modelName]['size'], curOffet)
        curOffet += fileInfo[modelName]['size']
        fo.write(byte)
    # encrypt lua data
    for modelName in fileInfo:
        fo.write(fileInfo[modelName]['buffer'])

    fo.close()
    print "Compile lua success: %s" %(joinDir(scriptRoot, "game.rd"))

if __name__ == "__main__":
    projectDir = None
    key = None

    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:k:")
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-p":
            projectDir = a
        if o == "-k":
            key = a

    if projectDir == None:
        print __doc__
        print "Error: Required parameter '-p'"
        sys.exit(0)

    if key == None:
        print __doc__
        print "Error: Required parameter '-k'"
        sys.exit(0)

    # compile lua fils
    compileLuaFiles(projectDir, key)
