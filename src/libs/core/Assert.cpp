#include "Assert.hpp"
#include <arch/i686/IO.hpp>
#include <Debug.hpp>

bool Assert_(const char * cond,const char * file,int line,const char * func){
  Debug::Critical("Assert","Assertion Failed %s", cond);
  Debug::Critical("Assert","In file %s line %d function %s", file,line,func);
  arch::i686::Panic();

  return false;
}
