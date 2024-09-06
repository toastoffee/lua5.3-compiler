/**
  ******************************************************************************
  * @file           : expression_optimize.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/9/6
  ******************************************************************************
  */

#include "parser.hpp"
#include <parser/lua_number.hpp>

Expression *Parser::optimizeUnaryOp(UnopExpression *exp) {
    switch (exp->op) {
        case TokenId::TOKEN_OP_UNM:
            return optimizeUnm(exp);
        case TokenId::TOKEN_OP_NOT:
            return optimizeNot(exp);
        case TokenId::TOKEN_OP_BNOT:
            return optimizeBnot(exp);
        default:
            return exp;
    }
}

Expression *Parser::optimizeUnm(UnopExpression *exp) {
    if(isInstanceOf<IntegerExpression>(exp->exp)) {
        auto iExp = dynamic_cast<IntegerExpression *>(exp->exp);
        iExp->val = - iExp->val;
        return iExp;
    }
    if(isInstanceOf<FloatExpression>(exp->exp)) {
        auto fExp = dynamic_cast<FloatExpression *>(exp->exp);
        fExp->val = - fExp->val;
        return fExp;
    }
    return exp;
}

Expression *Parser::optimizeNot(UnopExpression *exp) {
    auto subExp = exp->exp;
    if(isInstanceOf<NilExpression>(subExp)
            || isInstanceOf<FalseExpression>(subExp)) {
        return new TrueExpression(exp->line);
    }
    if(isInstanceOf<TrueExpression>(subExp)
       || isInstanceOf<IntegerExpression>(subExp)
       || isInstanceOf<FloatExpression>(subExp)
       || isInstanceOf<StringExpression>(subExp)) {
        return new FalseExpression(exp->line);
    }
    return exp;
}

Expression *Parser::optimizeBnot(UnopExpression *exp) {
    if(isInstanceOf<IntegerExpression>(exp->exp)) {
        auto iExp = dynamic_cast<IntegerExpression *>(exp->exp);
        iExp->val = ~ iExp->val;
        return iExp;
    }
    if(isInstanceOf<FloatExpression>(exp->exp)) {
        auto fExp = dynamic_cast<FloatExpression *>(exp->exp);
        f64 f = fExp->val;
        if(LuaNumber::IsInteger(f)) {
            return new IntegerExpression(fExp->line, ~(int)f);
        }
    }

    return exp;
}
