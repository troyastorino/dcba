#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
    Matrix4f *matrix = new Matrix4f();
    m_matrices.push_back(Matrix4f::identity());
	// Initialize the matrix stack with the identity matrix.
}

void MatrixStack::clear()
{
    m_matrices.resize(1);
	// Revert to just containing the identity matrix.
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
	return m_matrices.back();
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
    Matrix4f back = m_matrices.back();
    Matrix4f newBack = m * back;
    m_matrices.push_back(newBack);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
    m_matrices.pop_back();
}
