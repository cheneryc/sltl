#pragma once


namespace sltl
{
namespace syntax
{
  // Forward declarations - sltl::syntax namespace
  class block;
  class io_block;
  class variable_declaration;
  class parameter_declaration;
  class reference;
  class temporary;
  class assignment_operator;
  class binary_operator;
  class conditional;
  class expression_statement;
  class parentheses;
  class list_separator;
  class function_call;
  class function_definition;
  class return_statement;

  template<typename T>
  class literal;

  class action
  {
  public:
    virtual bool operator()(syntax::block&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::io_block&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::variable_declaration&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::parameter_declaration&) { return get_default(); }
    virtual bool operator()(syntax::reference&) { return get_default(); }
    virtual bool operator()(syntax::temporary&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::assignment_operator&) { return get_default(); }
    virtual bool operator()(syntax::binary_operator&) { return get_default(); }
    virtual bool operator()(syntax::conditional&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::expression_statement&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::parentheses&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::list_separator&) { return get_default(); }
    virtual bool operator()(syntax::function_call&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::function_definition&, bool = true) { return get_default(); }
    virtual bool operator()(syntax::return_statement&, bool = true) { return get_default(); }

    template<typename T>
    bool operator()(syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual bool operator()(float) { return get_default(); }
    virtual bool operator()(double) { return get_default(); }
    virtual bool operator()(int) { return get_default(); }
    virtual bool operator()(unsigned int) { return get_default(); }
    virtual bool operator()(bool) { return get_default(); }

    virtual bool get_default() = 0;
  };

  template<typename T>
  class action_result : public action
  {
  public:
    virtual T get_result() const = 0;
  };

  class const_action
  {
  public:
    virtual bool operator()(const syntax::block&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::io_block&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::variable_declaration&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::parameter_declaration&) { return get_default(); }
    virtual bool operator()(const syntax::reference&) { return get_default(); }
    virtual bool operator()(const syntax::temporary&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::assignment_operator&) { return get_default(); }
    virtual bool operator()(const syntax::binary_operator&) { return get_default(); }
    virtual bool operator()(const syntax::conditional&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::expression_statement&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::parentheses&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::list_separator&) { return get_default(); }
    virtual bool operator()(const syntax::function_call&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::function_definition&, bool = true) { return get_default(); }
    virtual bool operator()(const syntax::return_statement&, bool = true) { return get_default(); }

    template<typename T>
    bool operator()(const syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual bool operator()(float) { return get_default(); }
    virtual bool operator()(double) { return get_default(); }
    virtual bool operator()(int) { return get_default(); }
    virtual bool operator()(unsigned int) { return get_default(); }
    virtual bool operator()(bool) { return get_default(); }

    virtual bool get_default() = 0;
  };

  template<typename T>
  class const_action_result : public const_action
  {
  public:
    virtual T get_result() const = 0;
  };
}
}
