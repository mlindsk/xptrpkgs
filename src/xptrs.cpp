#include <RcppArmadillo.h>
using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::XPtr<arma::Mat<short>> make(int i, int j) {
  arma::Mat<short>* ptr(new arma::Mat<short>(i, j));
  (*ptr).fill(1);
  Rcpp::XPtr<arma::Mat<short>> p(ptr, true);
  return p;
}

// [[Rcpp::export]]
void release(Rcpp::XPtr<arma::Mat<short>>& p) {
  p.release();
}
