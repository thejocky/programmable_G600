#include <tuple>
#include <type_traits>
#include <cstdint>

namespace instructions {

    // Concept to check if a type is an std::integral_constant with value 0
    template <typename T>
    concept IsZeroIndex = std::is_same_v<T, std::integral_constant<std::size_t, 0>>;

    // Ensure defined comparison operator
    template <typename T>
    concept Comparable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
    };
    template <typename T>
    concept LessComparable = requires(T a, T b) {
        { a < b } -> std::convertible_to<bool>;
    };
    template <typename T>
    concept GreaterComparable = requires(T a, T b) {
        { a > b } -> std::convertible_to<bool>;
    };
    template <typename T>
    concept LessEqualComparable = requires(T a, T b) {
        { a <= b } -> std::convertible_to<bool>;
    };
    template <typename T>
    concept GreaterEqualComparable = requires(T a, T b) {
        { a >= b } -> std::convertible_to<bool>;
    };


    template <typename T>
    struct Instruction_Base {
        Instruction_Base() {}
        virtual T evaluate()=0; 
    };

    struct Null_statement : public Instruction_Base<void> {
        Null_statement() {}
        virtual void evaluate() override {} 
    };


    // State ------------------------------------------------------------------

    template <typename T>
    struct State : public Instruction_Base<T> {
        State() {}
        virtual T evaluate() override {}
    };

    template <typename T>
    struct Variable : public State<T> {
        T val;
        Variable(T value) : val(value) {}
        T evaluate() override {return val;}
    };

    template <typename T>
    struct Literal : public State<T> {
        private:
        T val;
        public:
        Literal(T value) : val(value) {}
        T evaluate() override {return val;}
    };

    template <typename T>
    struct Set_statement : public Instruction_Base<void> {
        Variable<T>* variable;
        Instruction_Base<T>* value;

        Set_statement(Variable<T>* var, Instruction_Base<T>* val)
            : variable(var), value(val) {}

        void evaluate() override {
            variable->val = value->evaluate();
        }
    };

    


    // Control Flow -----------------------------------------------------------

    template <typename... Args>
    struct CodeBlock : public Instruction_Base<void> {
        std::tuple<Instruction_Base<Args>*...> instructions;
        
        CodeBlock(Instruction_Base<Args>*... statements):
            instructions(make_tuple(statements)...)
        {}
        CodeBlock *createCodeBlock(Instruction_Base<Args>*... statements) {
            return new CodeBlock(statements...);
        }

        void evaluate() override {
            std::apply([](auto&&... args) { (args->evaluate(), ...); }, instructions);
        }
    };

    template <typename... Args>
    struct If_statement : public Instruction_Base<void> {
        Instruction_Base<bool>* condition;
        CodeBlock<Instruction_Base<Args>*...> code;
        Instruction_Base<void>* else_statement;

        If_statement(Instruction_Base<bool>* cond, Instruction_Base<Args>*... statements, Instruction_Base<void>* chainedElse)
            : condition(cond), code(statements...), else_statement(chainedElse) {}

        void evaluate() override {
            if (condition->evaluate())
                code.execute();
            else if (else_statement)
                else_statement->evaluate();
        }
    };

    template <typename... Args>
    struct WhileLoop : public Instruction_Base<void> {
        Instruction_Base<bool>* condition;
        CodeBlock<Instruction_Base<Args>*...> code;

        WhileLoop(Instruction_Base<bool>* cond_, Instruction_Base<Args>*... statements)
            : condition(cond_), code(statements...) {}

        void evaluate() override {
            while (condition->evaluate()) {
                code.execute();
            }
        }
    };


    // Logical Instructions ---------------------------------------------------


    struct LogicalAnd : public Instruction_Base<bool> {
        Instruction_Base<bool>* left;
        Instruction_Base<bool>* right;

        LogicalAnd(Instruction_Base<bool>* l, Instruction_Base<bool>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() && right->evaluate();
        }
    };

    struct LogicalOr : public Instruction_Base<bool> {
        Instruction_Base<bool>* left;
        Instruction_Base<bool>* right;

        LogicalOr(Instruction_Base<bool>* l, Instruction_Base<bool>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() || right->evaluate();
        }
    };

    struct LogicalNot : public Instruction_Base<bool> {
        Instruction_Base<bool>* operand;

        LogicalNot(Instruction_Base<bool>* op)
            : operand(op) {}

        bool evaluate() override {
            return !operand->evaluate();
        }
    };

    template <Comparable T>
    struct EqualTo : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        EqualTo(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() == right->evaluate();
        }
    };

    template <Comparable T>
    struct NotEqualTo : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        NotEqualTo(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() != right->evaluate();
        }
    };

    template <LessComparable T>
    struct LessThan : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        LessThan(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() < right->evaluate();
        }
    };

    template <GreaterComparable T>
    struct GreaterThan : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        GreaterThan(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() > right->evaluate();
        }
    };

    template <LessEqualComparable T>
    struct LessThanOrEqualTo : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        LessThanOrEqualTo(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() <= right->evaluate();
        }
    };

    template <GreaterEqualComparable T>
    struct GreaterThanOrEqualTo : public Instruction_Base<bool> {
        Instruction_Base<T>* left;
        Instruction_Base<T>* right;

        GreaterThanOrEqualTo(Instruction_Base<T>* l, Instruction_Base<T>* r)
            : left(l), right(r) {}

        bool evaluate() override {
            return left->evaluate() >= right->evaluate();
        }
    };
}