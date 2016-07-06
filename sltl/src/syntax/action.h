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

  enum class action_return_t
  {
    step_in,
    step_over,
    stop
  };

  class action
  {
  public:
    virtual action_return_t operator()(syntax::block&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::io_block&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::variable_declaration&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::parameter_declaration&) { return get_default(); }
    virtual action_return_t operator()(syntax::reference&) { return get_default(); }
    virtual action_return_t operator()(syntax::temporary&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::assignment_operator&) { return get_default(); }
    virtual action_return_t operator()(syntax::binary_operator&) { return get_default(); }
    virtual action_return_t operator()(syntax::conditional&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::expression_statement&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::parentheses&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::list_separator&) { return get_default(); }
    virtual action_return_t operator()(syntax::function_call&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::function_definition&, bool = true) { return get_default(); }
    virtual action_return_t operator()(syntax::return_statement&, bool = true) { return get_default(); }

    template<typename T>
    action_return_t operator()(syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual action_return_t operator()(float) { return get_default(); }
    virtual action_return_t operator()(double) { return get_default(); }
    virtual action_return_t operator()(int) { return get_default(); }
    virtual action_return_t operator()(unsigned int) { return get_default(); }
    virtual action_return_t operator()(bool) { return get_default(); }

    virtual action_return_t get_default() = 0;
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
    virtual action_return_t operator()(const syntax::block&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::io_block&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::variable_declaration&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::parameter_declaration&) { return get_default(); }
    virtual action_return_t operator()(const syntax::reference&) { return get_default(); }
    virtual action_return_t operator()(const syntax::temporary&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::assignment_operator&) { return get_default(); }
    virtual action_return_t operator()(const syntax::binary_operator&) { return get_default(); }
    virtual action_return_t operator()(const syntax::conditional&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::expression_statement&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::parentheses&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::list_separator&) { return get_default(); }
    virtual action_return_t operator()(const syntax::function_call&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::function_definition&, bool = true) { return get_default(); }
    virtual action_return_t operator()(const syntax::return_statement&, bool = true) { return get_default(); }

    template<typename T>
    action_return_t operator()(const syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual action_return_t operator()(float) { return get_default(); }
    virtual action_return_t operator()(double) { return get_default(); }
    virtual action_return_t operator()(int) { return get_default(); }
    virtual action_return_t operator()(unsigned int) { return get_default(); }
    virtual action_return_t operator()(bool) { return get_default(); }

    virtual action_return_t get_default() = 0;
  };

  template<typename T>
  class const_action_result : public const_action
  {
  public:
    virtual T get_result() const = 0;
  };
}
}
