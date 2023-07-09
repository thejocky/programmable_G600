#include "../include/instructions.hpp"
#include <iostream>
#include <string>

#include <cassert>

void state_test() {
    // Test state
    auto variable = new instructions::Variable<int>(5);
    auto literal = new instructions::Literal<int>(10);
    assert(variable->evaluate()==5);
    assert(literal->evaluate()==10);

    // Test state modification
    auto inst = new instructions::Set_statement<int>(variable, literal);
    inst->evaluate();
    assert(variable->evaluate() == literal->evaluate());
}

void logical_test() {

    // Test opperators for logical and & or
    auto valA = new instructions::Variable<bool>(false);
    auto valB = new instructions::Variable<bool>(false);
    auto andOp = new instructions::LogicalAnd(valA, valB);
    auto orOp = new instructions::LogicalOr(valA, valB);
    auto notOp = new instructions::LogicalNot(valA);

    bool andTable[2][2] = {
        {false, false},
        {false, true}
    };
    bool orTable[2][2] = {
        {false, true},
        {true, true}
    };
    bool notTable[2] = {true, false};

    for (int i = 0; i < 2; i++) {
        valA->val = i==1;
        for (int j = 0; j < 2; j++) {
            valB->val = j==1;
            assert(andOp->evaluate() == andTable[i][j]);
            assert(orOp->evaluate() == orTable[i][j]);
        }
        assert(notOp->evaluate() == notTable[i]);
    }
}

void rational_test() {
    // Test opperators for logical and & or
    instructions::Variable<int> compairVals[3] = {
        new instructions::Variable<int>(-5),
        new instructions::Variable<int>(10),
        new instructions::Variable<int>(15),
    }
    auto 
    auto valC = new instructions
    auto andOp = new instructions::LogicalAnd(valA, valB);
    auto orOp = new instructions::LogicalOr(valA, valB);
    auto notOp = new instructions::LogicalNot(valA);

    bool andTable[2][2] = {
        {false, false},
        {false, true}
    };
    bool orTable[2][2] = {
        {false, true},
        {true, true}
    };
    bool notTable[2] = {true, false};

    for (int i = 0; i < 2; i++) {
        valA->val = i==1;
        for (int j = 0; j < 2; j++) {
            valB->val = j==1;
            assert(andOp->evaluate() == andTable[i][j]);
            assert(orOp->evaluate() == orTable[i][j]);
        }
        assert(notOp->evaluate() == notTable[i]);
    }
}

// void control_flow_test() {
//     // If statement test
//     auto variable = new instructions::Variable<int>(0);
//     auto if_statement = new instructions::If_statement(
//         new instructions::Literal<bool>(false),
//         set
//     )
// }

int main() {
    // auto variable = new instructions::Variable<int>(5);
    // auto output = new instructions::Variable<bool> (false);
    // auto instructions = instructions::CodeBlock::createCodeBlock(
    //     new instructions::If_statement(
    //         new instructions::LessThan<int>(variable, new instructions::Literal<int>(10)),
    //         new instructions::Set_statement(output, new instructions::Literal(false)),
    //         new instructions::Set_statement(output, new instructions::Literal(true)),
    //     )
        
    // );

    // instructions->evaluate();
    // static_assert(output->val == true);

    // variable->val = 15;

    // instructions->evaluate();
    // static_assert(output->val == false);

    state_test();
    logical_test();
    // return 1;

}