//
//  SF_FileSystem.cpp
//  SwipeFootball
//
//  Created by Sid on 07/07/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "SF_FileSystem.h"
#include <cassert>
#include <CoreFoundation/CoreFoundation.h>

SF_File::SF_File(const std::string &file, const std::string &extension) :
file_(file),
extension_(extension)
{}

std::string SF_File::GetPath() const
{
    CFBundleRef bundle = CFBundleGetMainBundle();    
    CFStringRef resourceName = CFStringCreateWithCString(kCFAllocatorDefault, file_.c_str(), kCFStringEncodingUTF8);
    CFStringRef resourceType = CFStringCreateWithCString(kCFAllocatorDefault, extension_.c_str(), kCFStringEncodingUTF8);
    CFURLRef resourceURL = CFBundleCopyResourceURL(bundle, resourceName, resourceType, NULL);
    assert(resourceURL); // No resource in bundle
    CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceURL, kCFURLPOSIXPathStyle);
    
    std::string ret(CFStringGetCStringPtr(resourcePath, kCFStringEncodingUTF8));
    
    CFRelease(resourcePath);
    CFRelease(resourceURL);
    CFRelease(resourceType);
    CFRelease(resourceName);
    
    return ret;
}

#include <fstream>
#include <sstream>

std::string SF_FileSystem::ContentsOfFile(const SF_File &file)
{
    std::ifstream fin(file.GetPath());
    std::stringstream ss;
    ss << fin.rdbuf();
    
    return ss.str();
}
