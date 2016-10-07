#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    ProjectCreator --

SYNOPSIS
    ProjectCreator [-h] -p packageName [-l]

    -h show help
    -p packageName: com.mydomain.gamename
            Package name should use a full english characters with lowercase.
            The Prefix is always a top-level domain.
    -l default screen orientation is portrait, this change it to landscape
"""

import os
import sys
import getopt
import subprocess
import shutil
import re

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.path.split(scriptRoot)[0]

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def checkPackageName(pname):
    names = pname.split('.')
    if len(names) < 3:
        return None
    for name in names:
        if False == re.match('^[0-9a-z]+$',name.lower()):
            return None
    return pname.lower()

def copyCommon(src, dest, landscape):
    shutil.copytree(joinDir(src, "common"), joinDir(dest, "common"))
    
    shutil.rmtree(joinDir(dest, "common", "res"))
    os.mkdir(joinDir(dest, "common", "res"))
    
    shutil.rmtree(joinDir(dest, "common", "lua", "game"))
    os.mkdir(joinDir(dest, "common", "lua", "game"))

    # rebuild mainscene.lua
    output = open(joinDir(dest, "common", "lua", "game", "mainscene.lua"), 'w')
    output.write(r'''local MainScene = class("MainScene", rd.scene)

function MainScene:ctor()
    -- call super.ctor first
    MainScene.super.ctor(self)

    rd.label.TTF.new("Hello World!", 40, "Arial", {0, 0, 93, 255}, rd.label.CENTER)
	:addTo(self)

    -- create success
    return self
end

return MainScene''')
    output.close()

    # screenOrientation setting for startup.cpp
    inputFile = open(joinDir(dest, "common", "cpp", "startup.cpp"), 'r')
    lines = inputFile.readlines()
    inputFile.close()

    outputFile = open(joinDir(dest, "common", "cpp", "startup.cpp"), 'w')
    for line in lines:
        if line.find("Rapid2D_setResolutionModeSize") < 0:
            outputFile.write(line)
        else:
            if landscape:
                outputFile.write('    Rapid2D_setResolutionModeSize(1, 960, 640);\n')
            else:
                outputFile.write('    Rapid2D_setResolutionModeSize(1, 640, 960);\n')
    outputFile.close()

def copyWin32(src, dest, projectName, landscape):
    # mkdir and copy files, change "test" in filename.
    os.mkdir(joinDir(dest, "win32"))
    for name in os.listdir(joinDir(src, "win32")):
        if name.startswith("test."):
            inputFile = open(joinDir(src, "win32", name), "r")
            lines = inputFile.readlines()
            inputFile.close()

            newName = re.sub('test', projectName, name)
            outputFile = open(joinDir(dest, "win32", newName), "w")
            for line in lines:
                # fix screenOrientation in test.cpp
                if name.endswith(".cpp") and landscape:
                    if line.find("#define WIN_WIDTH") >= 0:
                        line = "#define WIN_WIDTH (800.0)\n"
                    if line.find("#define WIN_HEIGHT") >= 0:
                        line = "#define WIN_HEIGHT (480.0)\n"
                else:
                    line = re.sub('test', projectName, line)
                outputFile.write(line)
            outputFile.close()
        elif False == os.path.isdir(joinDir(src, "win32", name)):
            shutil.copy(joinDir(src, "win32", name),  joinDir(dest, "win32", name))

def copyiOS(src, dest, names, landscape):
    os.mkdir(joinDir(dest, "ios"))
    projectName = names[len(names) - 1]
    for item in os.walk(joinDir(src, "ios")):
        # not copy dir: xcworkspace & xcuserdata
        if item[0].find("xcworkspace") >= 0 or item[0].find("xcuserdata") >= 0:
            continue
        destDir = re.sub('test', projectName, item[0])
        destDir = re.sub('samples', "projects", destDir)
        if not os.path.exists(destDir):
            os.mkdir(destDir)
        for name in item[2]:
            # not copy .DS_Store
            if name.find("DS_Store") >= 0:
                continue
            destName = re.sub('test', projectName, name)
            
            inputFile = open(joinDir(item[0], name), "r")
            lines = inputFile.readlines()
            inputFile.close()

            outputFile = open(joinDir(destDir, destName), "w")
            for line in lines:
                if name == "project.pbxproj":
                    index = line.find("PRODUCT_BUNDLE_IDENTIFIER")
                    if index >= 0:
                        line = line[0:index] + "PRODUCT_BUNDLE_IDENTIFIER = " + ".".join(names) + ";\n"
                    else:
                        line = re.sub("test", projectName, line)
                elif name == "Info.plist" and landscape:
                    index = line.find("Portrait")
                    if index >= 0:
                        line = "\t\t<string>UIInterfaceOrientationLandscapeLeft</string>\n" \
                            + "\t\t<string>UIInterfaceOrientationLandscapeRight</string>\n"
                elif name == "ViewController.mm" and landscape:
                    index = line.find("return UIInterfaceOrientationMaskPortrait")
                    if index >= 0:
                        line = "    return UIInterfaceOrientationMaskLandscape;\n"
                outputFile.write(line)
            outputFile.close()

def copyMac(src, dest, names, landscape):
    os.mkdir(joinDir(dest, "mac"))
    projectName = names[len(names) - 1]
    for item in os.walk(joinDir(src, "mac")):
        # not copy dir: xcworkspace & xcuserdata
        if item[0].find("xcworkspace") >= 0 or item[0].find("xcuserdata") >= 0:
            continue
        destDir = re.sub('test', projectName, item[0])
        destDir = re.sub('samples', "projects", destDir)
        if not os.path.exists(destDir):
            os.mkdir(destDir)
        for name in item[2]:
            # not copy .DS_Store
            if name.find("DS_Store") >= 0:
                continue
            destName = re.sub('test', projectName, name)
            
            inputFile = open(joinDir(item[0], name), "r")
            lines = inputFile.readlines()
            inputFile.close()

            outputFile = open(joinDir(destDir, destName), "w")
            for line in lines:
                if name == "project.pbxproj":
                    index = line.find("PRODUCT_BUNDLE_IDENTIFIER")
                    if index >= 0:
                        line = line[0:index] + "PRODUCT_BUNDLE_IDENTIFIER = " + ".".join(names) + ";\n"
                    else:
                        line = re.sub("test", projectName, line)
                elif name == "MainMenu.xib" or name == "AppDelegate.m":
                    line = re.sub("test", projectName, line)
                elif name == "RDGLView.h" and landscape:
                    index = line.find("#define GLVIEW_WIDTH")
                    if index >= 0:
                        line = "#define GLVIEW_WIDTH (800)\n"
                    index = line.find("#define GLVIEW_HEIGHT")
                    if index >= 0:
                        line = "#define GLVIEW_HEIGHT (480)\n"
                outputFile.write(line)
            outputFile.close()

def copyAndroid(src, dest, names, landscape):
    shutil.copytree(joinDir(src, "android"), joinDir(dest, "android"))
    
    # change package dir name
    destAppJavaDir = joinDir(dest, "android", "app", "src", "main", "java")
    os.rename(joinDir(destAppJavaDir, "org"), joinDir(destAppJavaDir, names[0]))
    os.rename(joinDir(destAppJavaDir, names[0], "rapid2d"), joinDir(destAppJavaDir, names[0], names[1]))
    os.rename(joinDir(destAppJavaDir, names[0], names[1], "test"), joinDir(destAppJavaDir, names[0], names[1], names[2]))
    
    # app/build.gradle
    inputFile = open(joinDir(dest, "android", "app", "build.gradle"), "r")
    lines = inputFile.readlines()
    inputFile.close()

    outputFile = open(joinDir(dest, "android", "app", "build.gradle"), "w")
    for line in lines:
        index = line.find("applicationId")
        if index >= 0:
            line = line[0:index] + "applicationId \"" + ".".join(names) + "\"\n"
        outputFile.write(line)
    outputFile.close()
    
    # app/src/main/AndroidManifest.xml
    inputFile = open(joinDir(dest, "android", "app", "src", "main", "AndroidManifest.xml"), "r")
    lines = inputFile.readlines()
    inputFile.close()

    outputFile = open(joinDir(dest, "android", "app", "src", "main", "AndroidManifest.xml"), "w")
    for line in lines:
        index = line.find("package")
        if index >= 0:
            line = line[0:index] + "package=\"" + ".".join(names) + "\">\n"
        elif landscape:
            line = re.sub("portrait", "landscape", line)
        outputFile.write(line)
    outputFile.close()

    # app/src/main/java/xxx/xxx/xxx/MainActivity.java
    inputFile = open(joinDir(destAppJavaDir, names[0], names[1], names[2], "MainActivity.java"), "r")
    lines = inputFile.readlines()
    inputFile.close()

    outputFile = open(joinDir(destAppJavaDir, names[0], names[1], names[2], "MainActivity.java"), "w")
    lines[0] = "package " + ".".join(names) + ";\n"
    for line in lines:
        outputFile.write(line)
    outputFile.close()

    # app/src/main/res/values/strings.xml
    inputFile = open(joinDir(dest, "android", "app", "src", "main", "res", "values", "strings.xml"), "r")
    lines = inputFile.readlines()
    inputFile.close()

    outputFile = open(joinDir(dest, "android", "app", "src", "main", "res", "values", "strings.xml"), "w")
    for line in lines:
        line = re.sub("Test", names[2].capitalize(), line)
        outputFile.write(line)
    outputFile.close()

def createProject(packageName, landscape):
    # test project dir
    testProjDir = joinDir(engineRoot, "samples", "test")
    # new project dir
    projectDir = joinDir(engineRoot, "projects")
    if not os.path.exists(projectDir):
        os.mkdir(projectDir)

    names = packageName.split('.')
    projectName = names[len(names) - 1]
    newProjDir = joinDir(projectDir, projectName)
    
    if os.path.exists(newProjDir):
        print "Error: %s is exist!" %(newProjDir)
        return
    os.mkdir(newProjDir)

    print "Start Copying common ..."
    copyCommon(testProjDir, newProjDir, landscape)
    print "Start Copying win32 ..."
    copyWin32(testProjDir, newProjDir, projectName, landscape)
    print "Start Copying iOS ..."
    copyiOS(testProjDir, newProjDir, names, landscape)
    print "Start Copying Mac ..."
    copyMac(testProjDir, newProjDir, names, landscape)
    print "Start Copying Android ..."
    copyAndroid(testProjDir, newProjDir, names, landscape)
    print "Create project successful!"

if __name__ == "__main__":
    packageName = None
    landscape = False

    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:l")
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
            packageName = a
        if o == "-l":
            landscape = True

    if packageName == None:
        print __doc__
        print "Error: Required parameter '-p'"
        sys.exit(0)

    packageName = checkPackageName(packageName)
    if packageName == None:
        print __doc__
        print "Error: invalid packageName"
        sys.exit(0)

    # make a new project
    createProject(packageName, landscape)
