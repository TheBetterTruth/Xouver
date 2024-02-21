#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <XNI.h>

enum node_type {
    EOF_NODE,
    NUM_NODE,
    CHAR_NODE,
    BOOL_NODE,
    STRING_NODE,
    IDENT_NODE,
    BINOP_NODE,
    UNOP_NODE,
    VAR_DECL_NODE,
    FUNC_DECL_NODE,
    ASSIGN_NODE,
    CONDITION_NODE,
    MEMBER_NODE,
    ELEMENT_NODE,
    CALL_NODE,
    BLOCK_NODE,
    RETURN_NODE,
    IF_NODE,
    WHILE_NODE,
    FOR_NODE,
    CLASS_NODE
};

struct node {
    const node_type type;

    node(const node_type _Type) : type(_Type) {}
    virtual ~node() {}

    virtual void print() = 0;
};

typedef std::unique_ptr<node> node_ptr;

struct num_node : node {
    const double value;

    num_node(const double _Value) : node(NUM_NODE), value(_Value) {}
    ~num_node() {}

    void print() override {
        std::cout << value;
    }
};

struct char_node : node {
    const double value;

    char_node(const double _Value) : node(CHAR_NODE), value(_Value) {}
    ~char_node() {}

    void print() override {
        std::cout << value;
    }
};

struct string_node : node {
    const double value;

    string_node(const double _Value) : node(STRING_NODE), value(_Value) {}
    ~string_node() {}

    void print() override {
        std::cout << value;
    }
};

struct bool_node : node {
    const double value;

    bool_node(const double _Value) : node(BOOL_NODE), value(_Value) {}
    ~bool_node() {}

    void print() override {
        std::cout << value;
    }
};

struct ident_node : node {
    const std::string ident;

    ident_node(const std::string _Ident) :
        node(IDENT_NODE), ident(_Ident) {}
    ~ident_node() {}

    void print() override {
        std::cout << ident;
    }
};

struct binop_node : node {
    node_ptr left;
    const char op;
    node_ptr right;

    binop_node(node_ptr _Left, const char _Op, node_ptr _Right) :
        node(BINOP_NODE), left(std::move(_Left)), op(_Op), right(std::move(_Right)) {}
    ~binop_node() {}

    void print() override {
        std::cout << "(";
        left->print();
        std::cout << " " << op << " ";
        right->print();
        std::cout << ")";
    }
};

struct unop_node : node {
    const char op;
    node_ptr expr;

    unop_node(const char _Op, node_ptr _Expr) :
        node(UNOP_NODE), op(_Op), expr(std::move(_Expr)) {}
    ~unop_node() {}

    void print() override {
        std::cout << op << "(";
        expr->print();
        std::cout << ")";
    }
};

struct var_decl_node : node {
    const std::string name;
    std::string value_type;
    node_ptr value;

    var_decl_node(const std::string _Name, std::string _ValueType, node_ptr _Value) :
        node(VAR_DECL_NODE), name(_Name), value_type(_ValueType), value(std::move(_Value)) {}
    ~var_decl_node() {}

    void print() override {
        std::cout << "var " << name << " : " << value_type << " = ";
        value->print();
    }

    std::string get_type() {
        return value_type;
    }
};

struct assign_node : node {
    node_ptr dest;
    node_ptr expr;

    assign_node(node_ptr _Dst, node_ptr _Expr) :
        node(ASSIGN_NODE), dest(std::move(_Dst)), expr(std::move(_Expr)) {}
    ~assign_node() {}

    void print() override {
        dest->print();
        std::cout << " = ";
        expr->print();
    }
};


struct condition_node : node {
    node_ptr left;
    std::string op;
    node_ptr right;

    condition_node(node_ptr _Left, const std::string op, node_ptr _Right) :
            node(CONDITION_NODE), left(std::move(_Left)), op(op), right(std::move(_Right)) {}
    ~condition_node() {}

    void print() override {
        std::cout << "(";
        left->print();
        std::cout << " " << op << " ";
        right->print();
        std::cout << ")";
    }
};

struct member_node : node {
    node_ptr object;
    node_ptr member;

    member_node(node_ptr _Object, node_ptr _Member) :
        node(MEMBER_NODE), object(std::move(_Object)), member(std::move(_Member)) {}
    ~member_node() {}

    void print() override {
        object->print();
        std::cout << ".";
        member->print();
    }
};

struct element_node : node {
    node_ptr expr;
    node_ptr element;

    element_node(node_ptr _Expr, node_ptr _Element) :
        node(ELEMENT_NODE), expr(std::move(_Expr)), element(std::move(_Element)) {}
    ~element_node() {}

    void print() override {
        expr->print();
        std::cout << "[";
        element->print();
        std::cout << "]";
    }
};

struct call_node : node {
    node_ptr callee;
    std::vector<node_ptr> args;

    call_node(node_ptr _Callee, std::vector<node_ptr> _Args) :
        node(CALL_NODE), callee(std::move(_Callee)), args(std::move(_Args)) {}
    ~call_node() {}

    void print() override {
        callee->print();
        std::cout << "(";

        for (int i = 0; i < args.size(); i++) {
            args[i]->print();

            if (i < args.size() - 1)
                std::cout << ", ";
        }

        std::cout << ")";
    }
};

struct func_decl_node : node {
    std::string name;
    std::vector<var_decl_node> params;
    node_ptr body;
    std::string return_type;

    func_decl_node(std::string _Name, std::vector<var_decl_node> _Params, node_ptr _Body, std::string _ReturnType) :
        node(FUNC_DECL_NODE), name(_Name), params(_Params), body(std::move(_Body)), return_type(_ReturnType) {}
    ~func_decl_node() {}

    void print() override {
        std::cout << "func " << name << "(";
        
        std::cout << ") ";
        body->print();
    }

    std::string get_signature() {
        std::string signature = name + "(";

        for (int i = 0; i < params.size(); i++) {
            signature += params[i].type;

            if (i < params.size() - 1)
                std::cout << ",";
        }

        signature += ")@" + return_type;
    }
};

struct block_node : node {
    std::vector<node_ptr> statements;

    block_node(std::vector<node_ptr> _Statements) :
        node(BLOCK_NODE), statements(std::move(_Statements)) {}
    ~block_node() {}

    void print() override {
        std::cout << "{\n";
        for (int i = 0; i < statements.size(); i++) {
            statements[i]->print();
            std::cout << "\n";
        }
        std::cout << "}";
    }
};

struct return_node : node {
    node_ptr expr;

    return_node(node_ptr _Expr) :
        node(RETURN_NODE), expr(std::move(_Expr)) {}
    ~return_node() {}

    void print() override {
        std::cout << "retur ";
        if (expr != nullptr)
            expr->print();
    }
};

struct if_node : node {
    node_ptr condition;
    node_ptr body;

    if_node(node_ptr _Condition, node_ptr _Body) :
        node(IF_NODE), condition(std::move(_Condition)), body(std::move(_Body)) {}
    
    void print() override {
        std::cout << "if (";
        condition->print();
        std::cout << ") ";
        body->print();
    }
};

struct while_node : node {
    node_ptr condition;
    node_ptr body;

    while_node(node_ptr _Condition, node_ptr _Body) :
        node(WHILE_NODE), condition(std::move(_Condition)), body(std::move(_Body)) {}

    void print() override {
        std::cout << "while (";
        condition->print();
        std::cout << ") ";
        body->print();
    }
};

struct for_node : node {
    node_ptr expr;
    node_ptr body;

    for_node(node_ptr _Expr, node_ptr _Body) :
        node (FOR_NODE), expr(std::move(_Expr)), body(std::move(_Body)) {}

    void print() override {
        std::cout << "for ";
        expr->print();
        std::cout << " ";
        body->print();
    }
};

struct class_node : node {
    std::string class_name;
    std::string class_path;
    std::vector<node_ptr> body;

    class_node(std::string _ClassName, std::string _ClassPath, std::vector<node_ptr> _Body) :
        node(CLASS_NODE), class_name(_ClassName), class_path(_ClassPath), body(std::move(_Body)) {}

    void print() override {
        std::cout << "class " << class_name << " {\n";
        for (int i = 0; i < body.size(); i++) {
            body[i]->print();
            std::cout << "\n";
        }
        std::cout << "}";
    }
};

#endif