//
//  utillity.cpp
//
//
//  Created by Farideh Fazayeli on 7/18/13.
//  Updated for modern R/C++ compatibility & safe Error Handling.
//

#include "utillity.h"

void mvrnorm(double *des, double *mu, double *cholCov, int dim, bool upper){
  
  int i;
  int inc = 1;
  double one = 1.0;
  //make some std norm draws
  for(i = 0; i < dim; i++)
    des[i] = rnorm(0.0, 1.0);
  
  //mult this vector by the lower triangle of the cholCov
  if(upper)
    F77_NAME(dtrmv)("U", "T", "N", &dim, cholCov, &dim, des, &inc, 1, 1, 1);
  else
    F77_NAME(dtrmv)("L", "N", "N", &dim, cholCov, &dim, des, &inc, 1, 1, 1);
  
  //add the mean to the result
  F77_NAME(daxpy)(&dim, &one, mu, &inc, des, &inc);
}

/*
Print an array, assumes array dimension is known
*/
void PrintArr(double *myArr, int nRow){
  for (int ii = 0; ii < nRow; ii++) {
    Rprintf("%5.3f ", myArr[ii]);
  }
  Rprintf("\n");
}

void PrintArr(int *myArr, int nRow){
  for (int ii = 0; ii < nRow; ii++) {
    Rprintf("%d ", myArr[ii]);
  }
  Rprintf("\n");
}

/*
Print a matrix stored in an array, assumes matrix dimension is known
*/
void printMat(float *myArr, int nRow, int nCol){
  for (int ii = 0; ii < nRow; ii++) {
    for (int jj = 0; jj < nCol; jj++) {
      Rprintf("%5.3f ", myArr[ii*nCol+jj]);
    }
    Rprintf("\n");
  }
}

void printMat(double *myArr, int nRow, int nCol){
  for (int ii = 0; ii < nRow; ii++) {
    for (int jj = 0; jj < nCol; jj++) {
      Rprintf("%5.3f ", myArr[ii*nCol+jj]);
    }
    Rprintf("\n");
  }
}

/*
Print a matrix from a given matrix, assumes matrix dimension is known
*/
void printMat(float **myMat, int nRow, int nCol){
  for (int ii = 0; ii < nRow; ii++) {
    for (int jj = 0; jj < nCol; jj++) {
      Rprintf("%5.3f ", myMat[ii][jj]);
    }
    Rprintf("\n");
  }
}

void printMat(int **myMat, int nRow, int nCol){
  for (int ii = 0; ii < nRow; ii++) {
    for (int jj = 0; jj < nCol; jj++) {
      Rprintf("%d ", myMat[ii][jj]);
    }
    Rprintf("\n");
  }
}

SEXP getListElement (SEXP list, const char *str)
{
  SEXP elmt = R_NilValue, names = getAttrib(list, R_NamesSymbol);
  int i;
  
  for (i = 0; i < length(list); i++) {
    if(strcmp(CHAR(STRING_ELT(names, i)), str) == 0) {
      elmt = VECTOR_ELT(list, i);
      break;
    }
  }
  return elmt;
}

/*
void dlmreadVec(char *filePath, matVecFloat &myMat, int &nRow, int nCol, int verbose)
*/
void dlmreadVec(string filePath, matVecFloat &myMat, int &nRow, int nCol, int verbose){
  vector<float>  myArr;
  FILE * pFile;
  pFile = fopen (filePath.c_str(), "r");
  
  // Vibe Coding 终极修复：使用 R 安全报错机制拦截空指针
  if (!pFile) {
    Rf_error("BHPMF 致命错误: 无法打开文件读取路径 '%s'。请检查文件是否存在！", filePath.c_str());
  }
  
  nRow = 0;
  float num;
  while(fscanf(pFile, "%f", &num) != EOF){
    myArr.push_back(num);
  }
  fclose(pFile);
  nRow = myArr.size()/nCol;
  
  myMat.resize(nRow);
  for(int ii = 0; ii < nRow; ii++){
    myMat[ii].resize(nCol);
    for (int jj = 0; jj < nCol; jj++)
      myMat[ii][jj] = myArr[ii*nCol+jj];
  }
  if (verbose) {
    Rprintf("There are exactly %i rows and %i columns in the matrix \n", nRow, nCol);
  }
}

/*
void dlmreadVec(char *filePath, matVecInt &myMat, int &nRow, int nCol, int verbose)
*/
void dlmreadVec(string filePath, matVecInt &myMat, int &nRow, int nCol, int verbose){
  vector<int> myArr;
  FILE * pFile;
  pFile = fopen (filePath.c_str(), "r");
  
  // Vibe Coding 终极修复
  if (!pFile) {
    Rf_error("BHPMF 致命错误: 无法打开文件读取路径 '%s'。请检查文件是否存在！", filePath.c_str());
  }
  
  nRow = 0;
  int num;
  while(fscanf(pFile, "%d", &num) != EOF){
    myArr.push_back(num);
  }
  fclose(pFile);
  
  nRow = myArr.size()/nCol;
  myMat.resize(nRow);
  for(int ii = 0; ii < nRow; ii++){
    myMat[ii].resize(nCol);
    for (int jj = 0; jj < nCol; jj++)
      myMat[ii][jj] = myArr[ii*nCol+jj];
  }
  if (verbose) {
    Rprintf("There are exactly %i rows and %i columns in the matrix \n", nRow, nCol);
  }
}

/*
void dlmreadVec(char *filePath, matVecInt &myMat, int &nRow, int nCol)
*/
void dlmreadVec(string filePath, vector<int> &myArr){
  FILE * pFile;
  pFile = fopen (filePath.c_str(), "r");
  
  // Vibe Coding 终极修复
  if (!pFile) {
    Rf_error("BHPMF 致命错误: 无法打开文件读取路径 '%s'。请检查文件是否存在！", filePath.c_str());
  }
  
  int num;
  while(fscanf(pFile, "%d", &num) != EOF){
    myArr.push_back(num);
  }
  fclose(pFile);
}

void dlmwriteVec(const char *inFilePath, const char *outFilePath, int nRow,
                 int nCol, int gap, int burn, int nSams) {
  matVecFloat myMat;
  FILE * ipFile;
  ipFile = fopen (inFilePath, "r");
  
  // Vibe Coding 终极修复
  if (!ipFile) {
    Rf_error("BHPMF 致命错误: 无法读取临时文件 '%s'。这可能是 MCMC 迭代的中间文件未成功生成。", inFilePath);
  }
  
  float num;
  myMat.resize(nRow);
  for(int ii = 0; ii < nRow; ii++) {
    myMat[ii].resize(nCol);
  }
  
  int count = 1;
  for(int sam = 0; sam < nSams; sam++){
    for(int ii = 0; ii < nRow; ii++){
      for(int jj = 0; jj < nCol; jj++){
        fscanf(ipFile, "%f", &num);
        if(sam == gap){
          myMat[ii][jj] = num;
        }
        else if ((sam > gap) & ((sam-gap) % burn == 0)){
          myMat[ii][jj] += num;
          if (!ii & !jj) count++;
        }
      }
    }
  }
  fclose(ipFile);
  
  FILE * opFile;
  opFile = fopen (outFilePath, "w");
  
  // Vibe Coding 终极修复：拦截写入失败（最常见的崩溃点）
  if (!opFile) {
    Rf_error("BHPMF 致命错误: 无法在路径 '%s' 写入输出文件。请检查该文件夹是否存在、是否拼写错误、或是否有写入权限！", outFilePath);
  }
  
  for(int ii = 0; ii < nRow; ii++){
    for (int jj = 0; jj < nCol; jj++){
      fprintf(opFile,"%5.4f \t", myMat[ii][jj]/count);
    }
    fprintf(opFile,"\n");
  }
  fclose(opFile);
}
