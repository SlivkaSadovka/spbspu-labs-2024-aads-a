#include "conversion.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include "calculations.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "token.hpp"

strelyaev::Queue< strelyaev::detail::ExpressionUnit > strelyaev::parseString(std::istream& input)
{
  strelyaev::Queue< strelyaev::detail::ExpressionUnit > token_queue;
  strelyaev::detail::Token new_token;
  strelyaev::detail::TokenType type = strelyaev::detail::TokenType::NONE;
  std::string line = "";
  while (input)
  {
    input >> line;
    try
    {
      long long operand = std::stoll(line);
      new_token = strelyaev::detail::Token(operand);
      type = strelyaev::detail::TokenType::OPERAND;
    }
    catch (...)
    {
      char c = line[0];
      new_token = strelyaev::detail::Token(c);
      if (strelyaev::isBracket(line))
      {
        type = strelyaev::detail::TokenType::BRACKET;
      }
      if (strelyaev::isOperation(line))
      {
        type = strelyaev::detail::TokenType::OPERATION;
      }
    }
    strelyaev::detail::ExpressionUnit new_unit{new_token, type};
    token_queue.push(new_unit);
    if (input.peek() == '\n')
    {
      break;
    }
  }
  return token_queue;
}

strelyaev::Queue< strelyaev::detail::ExpressionUnit > strelyaev::makePostfix(Queue< detail::ExpressionUnit >& queue)
{
  strelyaev::Stack< strelyaev::detail::ExpressionUnit > temp_stack;
  strelyaev::Queue< strelyaev::detail::ExpressionUnit > postfix_queue;
  while (!queue.empty())
  {
    strelyaev::detail::ExpressionUnit unit = queue.drop();
    if (unit.type == strelyaev::detail::TokenType::OPERAND)
    {
      postfix_queue.push(unit);
    }
    if (unit.type == strelyaev::detail::TokenType::BRACKET)
    {
      if (unit.token.operation == '(')
      {
        temp_stack.push(unit);
      }
      if (unit.token.operation == ')')
      {
        strelyaev::detail::ExpressionUnit stack_peek = temp_stack.drop();
        while (stack_peek.token.operation != '(')
        {
          postfix_queue.push(stack_peek);
          stack_peek = temp_stack.drop();
        }
      }
    }
    if (unit.type == strelyaev::detail::TokenType::OPERATION)
    {
      if (temp_stack.empty())
      {
        temp_stack.push(unit);
        continue;
      }
      strelyaev::detail::ExpressionUnit stack_peek = temp_stack.drop();
      if (strelyaev::getPrecedence(unit.token.operation) > strelyaev::getPrecedence(stack_peek.token.operation))
      {
        temp_stack.push(stack_peek);
        temp_stack.push(unit);
      }
      else if (strelyaev::getPrecedence(unit.token.operation) <= strelyaev::getPrecedence(stack_peek.token.operation))
      {
        while (strelyaev::getPrecedence(unit.token.operation) <= strelyaev::getPrecedence(stack_peek.token.operation))
        {
          postfix_queue.push(stack_peek);
          if (temp_stack.empty())
          {
            break;
          }
          stack_peek = temp_stack.drop();
        }
        if (!temp_stack.empty())
        {
          temp_stack.push(stack_peek);
        }
        temp_stack.push(unit);
      }
    }
  }
  while (!temp_stack.empty())
  {
    strelyaev::detail::ExpressionUnit unit = temp_stack.drop();
    postfix_queue.push(unit);
  }
  return postfix_queue;
}

long long strelyaev::calculatePostfix(strelyaev::Queue< strelyaev::detail::ExpressionUnit >& queue)
{
  strelyaev::Stack< long long > processing_stack;
  if (queue.empty())
  {
    throw std::out_of_range("Empty queue is met");
  }
  while (!queue.empty())
  {
    strelyaev::detail::ExpressionUnit unit = queue.drop();
    if (unit.type == strelyaev::detail::TokenType::OPERAND)
    {
      processing_stack.push(unit.token.operand);
    }
    if (unit.type == strelyaev::detail::TokenType::OPERATION)
    {
      long long second = processing_stack.drop();
      if (processing_stack.empty())
      {
        throw std::invalid_argument("Something is wrong with postfix expression");
      }
      long long first = processing_stack.drop();
      long long result = calculateOperation(first, second, unit.token.operation);
      processing_stack.push(result);
    }
  }
  long long result = processing_stack.drop();
  if (!processing_stack.empty())
  {
    throw std::invalid_argument("Something is wrong with postfix expression");
  }
  return result;
}