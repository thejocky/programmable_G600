#include <tuple>
#include <type_traits>
#include <exception>
#include <cstdint>

namespace instructions {

    enum RetType {
        Void,
        String,
        Int,
        Bool
    };

    // Get corrisponding RetType to template type T
    template<typename T>
    constexpr RetType toRetType() {
        switch(typeid(T)) {
            case typeid(void): return RetType::Void;
            case typeid(void): return RetType::Void;
            case typeid(void): return RetType::Void;
            case typeid(void): return RetType::Void;
            default: throw runtime_error("Unrecognized instruction return type.");
        }
    };

    // Concept to check if type is supported in RetType enum
    template<typename T>
    concept IsRetType =
        std::is_same<T, void> ||
        std::is_same<T, int> ||
        std::is_same<T, int> ||
        std::is_same<T, int>;


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



    template <IsRetType T>
    struct Instruction_Base {
        Instruction_Base() {}
        virtual T evaluate()=0; 
    };

    // Store Instruction Base of any return type
    class PackInstruction {
        void* instruction_;
        RetType id_;
        public:

        template<IsRetType T>
        PackInstruction(Instruction_Base<T> *inst) :
            instruction_(inst), id_(toRetType<T>)
        {}
        
        template<IsRetType T>
        Instruction_Base<T> *unpack() {
            if (toRetType<T> == id_) return nullptr;
            return static_cast<Instruction_Base<T>*>(instruction_);
        }
    };

    template <IsRetType... Args>
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

    template <IsRetType T>
    struct Variable : public Instruction_Base<T> {
        T val;
        Variable(T value) : val(value) {}
        T evaluate() {return val;}
    };

    template <IsRetType... Args>
    struct if_statement : public Instruction_Base<void> {
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