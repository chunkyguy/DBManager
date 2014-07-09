//
//  SF_FileSystem.h
//  SwipeFootball
//
//  Created by Sid on 07/07/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#ifndef __SwipeFootball__SF_FileSystem__
#define __SwipeFootball__SF_FileSystem__
#include <string>

/** File located inside a app bundle */
class SF_File {
public:
    SF_File(const std::string &file, const std::string &extension);
    std::string GetPath() const;
    
private:
    std::string file_;
    std::string extension_;
};

class SF_FileSystem {
public:
    /** Read the contents of file */
    std::string ContentsOfFile(const SF_File &file);
};

#endif /* defined(__SwipeFootball__SF_FileSystem__) */
