#pragma once

#include "../Constants.hpp"

#include <iostream>
/* Types of tokens - to be able easily distinguish different tokens */
enum class EToken { Null, Operator, Operand };
enum class EBracket { Null, Left, Right };
enum class EOperator { Null, Unary, Binary, Bracket };
enum class EUnaryOp { Null, Minus, Plus, SFunction };
enum class EBinaryOp { Null, Minus, Plus, Multi, Divis, Exp };
enum class EOperand { Null, Number, String, CellLink, AFunction };
enum class EFunAggregate { Null, Max, Sum, Average };
enum class EFunSimple { Null, Sin, Cos, Abs, Ln, Log, Floor, Ceil };

/** Class CToken represents an element in a formula expression */

class CToken {
 public:
	/** Default constructor */
	CToken(): m_Token(EToken::Null), m_Bracket(EBracket::Null), m_Operator(EOperator::Null), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::Null), 
	m_FunAggregate(EFunAggregate::Null), m_FunSimple(EFunSimple::Null) { // DELETE
m_StringValue = "Def";}

	/** Constructor for brackets */
	CToken(EBracket bracket): m_Token(EToken::Operator), m_Bracket(bracket), m_Operator(EOperator::Bracket), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::Null), m_OperatorPriority(1) { //  DELETE
m_StringValue = "Bra";}

	/** Constructor for binary operators */
	CToken(EBinaryOp binaryOperator): m_Token(EToken::Operator), m_Bracket(EBracket::Null), m_Operator(EOperator::Binary), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(binaryOperator), m_Operand(EOperand::Null) { // DELETE
 m_StringValue = "BOp";
		switch (binaryOperator) {
			case EBinaryOp::Minus:
			case EBinaryOp::Plus:
				m_OperatorPriority = 2;
				break;
			case EBinaryOp::Multi:
			case EBinaryOp::Divis:
				m_OperatorPriority = 3;
				break;
			case EBinaryOp::Exp:
				m_OperatorPriority = 4;
				break;
			default:
				m_OperatorPriority = 0;
				break;
		}
	}

	/** Constructor for unary operators */
	CToken(EUnaryOp unaryOperator): m_Token(EToken::Operator), m_Bracket(EBracket::Null), m_Operator(EOperator::Unary), 
	m_UnaryOperator(unaryOperator), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::Null), m_OperatorPriority(5)  { // DELETE
	m_StringValue = "UOr";}

	/** Constructor for m_CellLink operand */
	CToken(TPosition cellPosition): m_Token(EToken::Operand), m_Bracket(EBracket::Null), m_Operator(EOperator::Null), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::CellLink), m_CellLink(cellPosition) {// DELETE
	 m_StringValue = "Cli";}

	/** Constructor for m_NumericValue operand */
	CToken(double number): m_Token(EToken::Operand), m_Bracket(EBracket::Null), m_Operator(EOperator::Null), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::Number), m_NumValue(number) {// DELETE
	 m_StringValue = "Num";}

	/** Constructor for m_StringValue operand */
	CToken(std::string stringValue): m_Token(EToken::Operand), m_Bracket(EBracket::Null), m_Operator(EOperator::Null), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::String), m_StringValue(stringValue)  { // DELETE
	 m_StringValue = "Str";}

	/** Constructor for aggregate function */
	CToken(EFunAggregate function, std::pair<TPosition, TPosition> cellPositions): m_Token(EToken::Operand), m_Bracket(EBracket::Null), m_Operator(EOperator::Null), 
	m_UnaryOperator(EUnaryOp::Null), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::AFunction), 
	m_FunAggregate(function), m_FunSimple(EFunSimple::Null), m_CellLinks(cellPositions) { //DELETE
	 m_StringValue = "Agg";}

	/** Constructor for simple function with cell link */
	CToken(EFunSimple function):m_Token(EToken::Operator), m_Bracket(EBracket::Null), m_Operator(EOperator::Unary), 
	m_UnaryOperator(EUnaryOp::SFunction), m_BinaryOperator(EBinaryOp::Null), m_Operand(EOperand::Null), 
	m_FunAggregate(EFunAggregate::Null), m_FunSimple(function), m_OperatorPriority(5) { //DELETE
	 m_StringValue = "SF";}



	EToken m_Token;
	EBracket m_Bracket;
	EOperator m_Operator;
	EUnaryOp m_UnaryOperator;
	EBinaryOp m_BinaryOperator;
	EOperand m_Operand;
	EFunAggregate m_FunAggregate;
	EFunSimple m_FunSimple;

	std::string m_StringValue;
	double m_NumValue;
	TPosition m_CellLink;
	std::pair<TPosition, TPosition> m_CellLinks;
	unsigned m_OperatorPriority;

	EToken getType() const {
		return m_Token;
	}

	unsigned getOperatorPriority() const {
		return m_OperatorPriority;
	}

	friend std::ostream & operator << (std::ostream & o, CToken t	) {
		o << t.m_StringValue;
		return o;
	}
	// could implement getters and make everything private - maybe at the end, it is not necessary
};
