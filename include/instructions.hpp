#include <tuple>
#include <type_traits>

namespace instructions {

    // Concept to check if a type is an std::integral_constant with value 0
    template <typename T>
    concept IsZeroIndex = std::is_same_v<T, std::integral_constant<std::size_t, 0>>;


    template <typename T>
    struct Instruction_base {
        Instruction_base()=delete;
        virtual T evaluate()=0; 
    };

    template <typename... Args>
    struct CodeBlock {
        std::tuple<Instruction_Base<Args>*...> instructions;

        CodeBlock()

        // Base case for instruction iteration
        template <std::size_t Index = sizeof...(Args) - 1>
        inline void executeInstructions(std::tuple<Instruction_Base<Args>*...> instructions) {
            std::get<Index>(instructions).execute();
        }

        // Recursive case for instruction iteration
        template <std::size_t Index = sizeof...(Args) - 1>
        requires (!IsZeroIndex<std::integral_constant<std::size_t, Index>>)
        inline void executeInst(std::tuple<Instruction_Base<Args>*...> instructions) {
            std::get<Index>(instructions).execute();
            executeInst<Index + 1>(instructions);
        }
    };

    template <typename T>
    struct Variable : public Instruction_base<T> {
        T val;
        T evalutate() {return val;}
    };

    template <typename... Args>
    struct if_statement : public Instruction_base<void> {
        Instruction_base<void> *chainedStatements_;
        codeBlock<Args> code;
    

        if_statement()=0;
        generate(Instruction_Base<Args>*... instructions) {
            
        }
    }

}