#include <tuple>


namespace instructions {

    template <typename T>
    struct Instruction_base {
        Instruction()=0
        virtual T evaluate()=0; 
    };

    template <typename... Args>
    struct codeBlock {
        std::tuple<Instruction_base<Args>...> instructions;
        template 
    };

    template <typename T>
    struct Variable : public Instruction_base<T> {
        T val;
        T evalutate() {return val;}
    };

    template <typename... Args>
    struct if_statement : public Instruction_base<void> {
        Instruction_base<void> *chainedStatements_;
        std::tuple<Instruction_base<Args>...> instructions;

        template <size_t I = 0, typename... Ts>
typename enable_if<(I < sizeof...(Ts)),
                   void>::type
        executeInst()

        if_statement()=0;
        generate(Instruction_Base<Args>... instructions) {

        }
    }

}