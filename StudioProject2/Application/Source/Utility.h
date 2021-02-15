#ifndef UTILITY_H
#define UTILTIY_H
#include "Mtx44.h"
#include "Vertex.h"

Position operator*(const Mtx44& lhs, const Position& rhs);

#endif