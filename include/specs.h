#ifndef SPECS_H
#define SPECS_H

#include "tools.h"

struct BooleanExpression;
struct Term;

// Declare function prototypes here
void canonicalSOP(BooleanExpression expression);
void canonicalPOS(BooleanExpression expression);
void inverseCanonicalSOP(BooleanExpression expression);
void inverseCanonicalPOS(BooleanExpression expression);
vector<string> minimizedSOP(BooleanExpression expression);
vector<string> minimizedPOS(BooleanExpression expression);
vector<string> primeImplicants(BooleanExpression expression);
vector<string> essentialPrimeImplicants(BooleanExpression expression);
void onSetMinterms(BooleanExpression expression);
void onSetMaxterms(BooleanExpression expression);
void asciiSOP(BooleanExpression expression);

#endif // SPECS_H
