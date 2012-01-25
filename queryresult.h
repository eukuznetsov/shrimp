#ifndef QUERYRESULT_H
#define QUERYRESULT_H

template <typename T> class QueryResult{
  T result;
public:
  QueryResult(T *res) { result = *res;}
  virtual char* fetch();
};

#endif // QUERYRESULT_H
