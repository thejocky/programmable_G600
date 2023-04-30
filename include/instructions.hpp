#include <tuple>
#include <type_traits>

namespace instructions {

    // Concept to check if a type is an std::integral_constant with value 0
    template <typename T>
    concept IsZeroIndex = std::is_same_v<T, std::integral_constant<std::size_t, 0>>;


    template <typename T>
    struct Instruction_Base {
        Instruction_Base()=delete;
        virtual T evaluate()=0; 
    };

    template <typename... Args>
    struct CodeBlock : public Instruction_Base<void> {
        std::tuple<Instruction_Base<Args>*...> instructions;

        CodeBlock(Instruction_Base<Args>*... statements):
            instructions(make_tuple(statements)...)
        {}

        void evaluate() override {
            std::apply([](auto&&... args) { (args->evaluate(), ...); }, instructions);
        }
    };

    template <typename T>
    struct Variable : public Instruction_Base<T> {
        T val;
        Variable(T value) : val(value) {}
        T evaluate() {return val;}
    };

    template <typename... Args>
    struct if_statement : public Instruction_Base<void> {
        Instruction_Base<bool>* condition;
        CodeBlock<Instruction_Base<Args>*...> code;
        Instruction_Base<void>* else_statement;

        if_statement(Instruction_Base<bool>* cond, Instruction_Base<Args>*... statements)
            : condition(cond), code(statements...) {}

        void evaluate() override {
            if (condition->evaluate())
                code.execute();
            else if (else_statement)
                else_statement->evaluate();
        }
    };
}