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
  class parameter_list;
  class reference;
  class temporary;
  class operator_unary;
  class operator_binary;
  class operator_component_access;
  class conditional;
  class constructor_call;
  class expression_statement;
  class expression_list;
  class parentheses;
  class function_call;
  class function_definition;
  class return_statement;
  class intrinsic_call;
  class intrinsic_declaration;

  template<typename T>
  class literal;

  enum class action_return_t
  {
    step_in,
    step_over,
    step_out,
    stop
  };

  class action
  {
  public:
    virtual action_return_t operator()(syntax::block&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::io_block&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::variable_declaration&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::parameter_declaration&) { return get_default(false); }
    virtual action_return_t operator()(syntax::parameter_list&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::reference&) { return get_default(false); }
    virtual action_return_t operator()(syntax::temporary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::operator_unary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::operator_binary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::operator_component_access&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::conditional&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::constructor_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::expression_statement&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::expression_list&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::parentheses&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::function_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::function_definition&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::return_statement&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::intrinsic_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(syntax::intrinsic_declaration&, bool is_start = true) { return get_default(is_start); }

    template<typename T>
    action_return_t operator()(syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual action_return_t operator()(float) { return get_default(false); }
    virtual action_return_t operator()(double) { return get_default(false); }
    virtual action_return_t operator()(int) { return get_default(false); }
    virtual action_return_t operator()(unsigned int) { return get_default(false); }
    virtual action_return_t operator()(bool) { return get_default(false); }

    virtual action_return_t get_default(bool is_start) = 0;
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
    virtual action_return_t operator()(const syntax::block&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::io_block&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::variable_declaration&, bool = true) { return get_default(false); }
    virtual action_return_t operator()(const syntax::parameter_declaration&) { return get_default(false); }
    virtual action_return_t operator()(const syntax::parameter_list&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::reference&) { return get_default(false); }
    virtual action_return_t operator()(const syntax::temporary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::operator_unary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::operator_binary&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::operator_component_access&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::conditional&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::constructor_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::expression_statement&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::expression_list&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::parentheses&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::function_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::function_definition&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::return_statement&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::intrinsic_call&, bool is_start = true) { return get_default(is_start); }
    virtual action_return_t operator()(const syntax::intrinsic_declaration&, bool is_start = true) { return get_default(is_start); }

    template<typename T>
    action_return_t operator()(const syntax::literal<T>& cd)
    {
      return (*this)(cd._t);
    }

  protected:
    virtual action_return_t operator()(float) { return get_default(false); }
    virtual action_return_t operator()(double) { return get_default(false); }
    virtual action_return_t operator()(int) { return get_default(false); }
    virtual action_return_t operator()(unsigned int) { return get_default(false); }
    virtual action_return_t operator()(bool) { return get_default(false); }

    virtual action_return_t get_default(bool is_start) = 0;
  };

  template<typename T>
  class const_action_result : public const_action
  {
  public:
    virtual T get_result() const = 0;
  };
}
}
