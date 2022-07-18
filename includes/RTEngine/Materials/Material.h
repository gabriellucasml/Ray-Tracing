//
// Created by gabriel on 07/06/22.
//

#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include <string>
#include <utility>
#include "../../Vector/color.h"

class Material{
private:
    std::string type;
    color col;
public:

    const std::string &getType() const {
        return type;
    }

    void setType(const std::string &type_) {
        this->type = type_;
    }

    const color &getCol() const {
        return col;
    }

    void setCol(const color &col_){
        this->col = col_;
    }
};
#endif //RT_MATERIAL_H
