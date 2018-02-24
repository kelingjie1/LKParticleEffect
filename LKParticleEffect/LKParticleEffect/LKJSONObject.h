//
//  LKJSONObject.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKJSONObject_h
#define LKJSONObject_h

#include <string>
#include <map>
#include <vector>
namespace LKKit
{
    enum LKJSONObjectType
    {
        LKJSONObjectTypeUnknow,
        LKJSONObjectTypeNumber,
        LKJSONObjectTypeString,
        LKJSONObjectTypeMap,
        LKJSONObjectTypeVector,
    };
    using namespace std;
    class LKJSONObject
    {
    public:
        LKJSONObjectType type;
        double valueNumber;
        string valueString;
        map<string,LKJSONObject> valueMap;
        vector<LKJSONObject> valueVector;
    };
}


#endif /* LKJSONObject_h */
