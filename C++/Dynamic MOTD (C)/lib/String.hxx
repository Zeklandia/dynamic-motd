// Copyright (C) 2004-2005 Vivien Mallet
//
// This file is part of Talos library.
// Talos library provides miscellaneous tools to make up for C++
// lacks and to ease C++ programming.
//
// Talos is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Talos is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License (file "license") for more details.
//
// For more information, please see the Talos home page:
//     http://spacetown.free.fr/lib/talos/


#ifndef TALOS_FILE_STRING_HXX

#include "Date.hxx"

namespace Talos
{

  template<typename T>
  string to_str(const T& input);

  string fill(const string& input, int l = 2, char c = ' ',
	      ostringstream::fmtflags flags = ostringstream::left);

  template<typename T>
  string to_str_fill(const T& input, int l = 2, char c = ' ',
		     ostringstream::fmtflags flags = ostringstream::left);

  template <class T> 
  void to_num(const string& s, T& num);

  template <class T>
  T to_num(const string& s);

  template <class T>
  void convert(const string& s, T& num);
  void convert(const string& s, string& num);

  template <class T>
  T convert(const string& s);
  
  string lower_case(string str);
  string upper_case(string str);
  
  bool is_num(const string& s);

  bool is_integer(const string& s);

  bool is_unsigned_integer(const string& s);

  bool is_date(const string& s);

  bool is_delta(const string& s);

  string find_replace(string str, string old_str, string new_str = "");

  string trim(string str, string delimiters = " \n\t");

  string trim_beg(string str, string delimiters = " \n\t");

  string trim_end(string str, string delimiters = " \n\t");

  template <class T>
  void split(string str, vector<T>& vect, string delimiters = " \n\t");
  vector<string> split(string str, string delimiters = " \n\t");

  template <class T>
  void split_markup(string str, vector<T>& elements, vector<bool>& is_markup,
		    string delimiters = "$");
  
  template <class T>
  void print(const vector<T>& v);
  
}  // namespace Talos.


#define TALOS_FILE_STRING_HXX
#endif
