#include <tuple>
#include <type_traits>
#include <exception>

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



    template <IsRetType T>
    struct Instruction_Base {
        Instruction_Base()=delete;
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