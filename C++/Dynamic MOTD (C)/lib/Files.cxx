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


#ifndef TALOS_FILE_FILES_CXX

#include "Files.hxx"

namespace Talos
{
  
  //! Tests whether a file exists.
  /*!
    \param file_name file name.
    \return true if the file exists, false otherwise.
    \note This function returns false if the user has not the rights to
    read the file, even if the file exists.
  */
  bool exists(string file_name)
  {
    ifstream file_stream(file_name.c_str(), ifstream::in);
    bool ans = file_stream.is_open();
    file_stream.close();
    
    return ans;
  }

  //! Returns a file size.
  /*!
    \param file_name file name.
    \return The file size in bytes.
  */
  unsigned long file_size(string file_name)
  {
    ifstream file_stream(file_name.c_str(), ifstream::in);
    streampos position = file_stream.tellg();
    file_stream.seekg(0, ios::end);

    return (file_stream.tellg() - position);
  }

  //! Returns a stream size.
  /*!
    \param stream the stream.
    \return The stream size in bytes.
  */
  unsigned long stream_size(istream& stream)
  {
    streampos position = stream.tellg();
    stream.seekg(0, ios::end);
    unsigned long length = stream.tellg() - position;
    stream.seekg(position, ios::beg);

    return length;
  }

  //! Checks whether a stream is empty.
  /*!
    \param stream the stream.
    \return true if the stream is empty, false otherwise.
  */
  bool is_empty(istream& stream)
  {
    streampos position = stream.tellg();
    istream::iostate state = stream.rdstate();

    stream.get();
    bool res = !stream.good();

    stream.clear(state);
    stream.seekg(position);

    return res;
  }

  //! Checks whether a stream contains an element.
  /*!
    Checks whether a stream contains an element that may be extracted
    through 'operator >>'.
    \param stream the stream.
    \return true if the stream has an element, false otherwise.
  */
  bool has_element(istream& stream)
  {
    streampos position = stream.tellg();
    istream::iostate state = stream.rdstate();

    string element;
    bool res = (stream >> element);

    stream.clear(state);
    stream.seekg(position);

    return res;
  }


  ///////////////
  // EXTSTREAM //
  ///////////////

  //! Default constructor.
  /*! Nothing is performed.
   */
  ExtStream::ExtStream():
    comments_("#%"), delimiters_(" \t"), searching_("")
  {
  }

  //! Main constructor.
  /*! Opens a file.
    \param file_name file to be opened.
  */
  ExtStream::ExtStream(string file_name,
		       string comments,
		       string delimiters):
    ifstream(file_name.c_str(), ifstream::binary), file_name_(file_name),
    comments_(comments), delimiters_(delimiters), searching_("")
  {
    if (!this->is_open())
      throw string("Unable to open file \"") + file_name + "\".";
  }

  //! Destructor.
  /*! Closes the stream.
   */
  ExtStream::~ExtStream()
  {
    this->close();
  }

  //! Checks whether a line should be discarded.
  /*!
    \param line line to be checked.
    \return true if the line should be discarded, false otherwise.
  */
  bool ExtStream::Discard(string line) const
  {
    size_t first = line.find_first_not_of(delimiters_);
    return ( (first == string::npos)
	     || (comments_.find_first_of(line[first]) != string::npos) );
  }

  //! Skips discarded lines.
  /*!
    Extracts discarded lines.
    \return A reference to the current stream.
  */
  ExtStream& ExtStream::SkipDiscarded()
  {
    streampos position;
    while ( (!is_empty(*this)) && (Discard(PeekFullLine(position))) )
      this->seekg(position);
    return *this;
  }

  //! Sets the characters considered as delimiters.
  /*!
    \param delimiters delimiters.
  */
  void ExtStream::SetDelimiters(string delimiters)
  {
    delimiters_ = delimiters;
  }

  //! Sets the characters that denote a comment line.
  /*!
    \param comments the characters that denote a comment line.
  */
  void ExtStream::SetComments(string comments)
  {
    comments_ = comments;
  }

  //! Returns the characters considered as delimiters..
  /*!
    \return Delimiters.
  */
  string ExtStream::GetDelimiters() const
  {
    return delimiters_;
  }

  //! Returns the characters that denote a comment line.
  /*!
    \return The characters that denote a comment line.
  */
  string ExtStream::GetComments() const
  {
    return comments_;
  }

  //! Returns the name of the file that was opened.
  /*!
    \return The name of the file that was opened.
  */
  string ExtStream::GetFileName() const
  {
    return file_name_;
  }

  //! Skips delimiters.
  /*!
    Extracts following delimiters from the string, until another character
    is found.
    \return A reference to the current stream.
  */
  ExtStream& ExtStream::SkipDelimiters()
  {
    while ( this->good()
	    && (delimiters_.find_first_of(char(this->peek())) != string::npos) )
      this->get();
    return *this;
  }

  //! Removes delimiters at both ends of a string.
  /*!
    Removes delimiters at the beginning and at the end of a string.
    \param str string.
    \return The string without delimiters at both ends.
  */
  string ExtStream::RemoveDelimiters(const string& str) const
  {
    string::size_type index = str.find_first_not_of(delimiters_);
    if (index == string::npos)
      return "";
    return str.substr(index,
		      str.find_last_not_of(delimiters_) - index + 1);
  }

  //! Skips discarded lines and delimiters.
  /*!
    Extracts discarded lines and delimiters.
    \return A reference to the current stream.
  */
  ExtStream& ExtStream::Skip()
  {
    this->SkipDiscarded();
    return this->SkipDelimiters();
  }

  //! Opens a file.
  /*!
    \param file_name file name.
    \param mode (optional) flags describing the requested I/O mode for the file.
    Default: in.
    \note If a file was previously opened, it is closed and the stream is cleared.
  */
  void ExtStream::Open(string file_name, openmode mode)
  {
    this->close();
    this->clear();
    this->open(file_name.c_str(), mode);

    file_name_ = file_name;

    if (!this->is_open())
      throw string("Unable to open file \"") + file_name + "\".";
  }

  //! Closes the current file.
  /*!
    \note The stream is cleared.
  */
  void ExtStream::Close()
  {
    this->close();
    this->clear();

    file_name_ = "";
  }

  //! Checks whether the stream is empty.
  /*!
    Checks whether the stream has still valid elements to be read.
    \return 'true' is the stream is empty, 'false' otherwise.
  */
  bool ExtStream::IsEmpty()
  {
    string tmp;
    return !this->PeekElement(tmp);
  }

  //! Rewinds the stream.
  /*!
    Goes back to the beginning of the stream and clears the control state.
    \return A reference to the current stream.
  */
  ExtStream& ExtStream::Rewind()
  {
    this->clear();
    this->seekg(0, ifstream::beg);

    return *this;
  }

  //! Returns the next line.
  /*!
    \return The next line.
  */
  string ExtStream::GetFullLine()
  {
    string line;

    std::getline(*this, line);

    return line;
  }

  //! Returns the next line.
  /*!
    \param line (output) the next line.
  */
  bool ExtStream::GetFullLine(string& line)
  {
    return std::getline(*this, line);
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \return The next line.
  */
  string ExtStream::PeekFullLine()
  {
    streampos position = this->tellg();
    iostate state = this->rdstate();

    string line;
    std::getline(*this, line);

    this->clear(state);
    this->seekg(position);

    return line;
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \param position (output) the position of the line following the next line.
    \return The next line.
  */
  string ExtStream::PeekFullLine(streampos& position)
  {
    streampos position_back = this->tellg();
    iostate state = this->rdstate();

    string line;
    std::getline(*this, line);

    position = this->tellg();

    this->clear(state);
    this->seekg(position_back);

    return line;
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \param line (output) the next line.
    \return true if a line has been found, false otherwise.
  */
  bool ExtStream::PeekFullLine(string& line)
  {
    streampos position = this->tellg();
    iostate state = this->rdstate();

    bool success = std::getline(*this, line);

    this->clear(state);
    this->seekg(position);

    return success;
  }

  //! Skips full lines.
  /*!
    \param nb number of lines to be skipped.
  */
  void ExtStream::SkipFullLines(int nb)
  {
    for (int i=0; i<nb; i++)
      this->GetFullLine();
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \return The next valid line.
  */
  string ExtStream::GetLine()
  {
    bool not_end;
    string line;
    string::size_type index(0), index_tmp;

    this->Skip();
    line = GetFullLine();

    while ( (not_end = ( (index_tmp = line.substr(index).find_first_of(comments_))
			 != string::npos ))
	    && (delimiters_.find_first_of(line[(index+=index_tmp)-1]) == string::npos)
	    && (not_end = (++index != line.size())) );

    if (not_end)
      index --;
    else
      index = line.size();
    
    while (delimiters_.find_first_of(line[--index]) != string::npos);

    return line.substr(0, index + 1);
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \param line (output) the next valid line.
  */
  bool ExtStream::GetLine(string& line)
  {
    bool not_end, success;
    string::size_type index(0), index_tmp;

    this->Skip();
    success = GetFullLine(line);

    while ( (not_end = ( (index_tmp = line.substr(index).find_first_of(comments_))
			 != string::npos ))
	    && (delimiters_.find_first_of(line[(index+=index_tmp)-1]) == string::npos)
	    && (not_end = (++index != line.size())) );

    if (not_end)
      index --;
    else
      index = line.size();
    
    while (delimiters_.find_first_of(line[--index]) != string::npos);

    line = line.substr(0, index + 1);

    return success;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \return The next valid line.
  */
  string ExtStream::PeekLine()
  {
    string line;

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    line = this->GetLine();

    this->clear(state);
    this->seekg(initial_position);

    return line;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \param position (output) the position of the line following the next valid line.
    \return The valid line.
  */
  string ExtStream::PeekLine(streampos& position)
  {
    streampos position_back = this->tellg();
    iostate state = this->rdstate();

    string line = this->GetLine();

    position = this->tellg();

    this->clear(state);
    this->seekg(position_back);

    return line;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \param line (output) the next valid line.
  */
  bool ExtStream::PeekLine(string& line)
  {
    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    bool success = this->GetLine(line);

    this->clear(state);
    this->seekg(initial_position);

    return success;
  }

  //! Skips valid lines.
  /*!
    \param nb number of lines to be skipped.
  */
  void ExtStream::SkipLines(int nb)
  {
    for (int i=0; i<nb; i++)
      this->GetLine();
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found.
    \return true if the element was found, false otherwise.
  */
  bool ExtStream::Find(string element)
  {
    searching_ = element;

    string elt;
    while (GetElement(elt) && elt!=element);

    if (elt == "")
      throw string("Error in ExtStream::Find: \"")
	+ element + string("\" not found in \"") + file_name_ + "\".";

    searching_ = "";

    return elt == element;
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found from the beginning of the stream.
    \return true if the element was found, false otherwise.
  */
  bool ExtStream::FindFromBeginning(string element)
  {
    this->Rewind();
    return this->Find(element);
  }

  //! Returns the next valid element.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \return The next valid element.
  */
  string ExtStream::GetElement()
  {
    streampos position;
    string element;
    string::size_type index, length;

    while ( (!is_empty(*this)) && (Discard(PeekFullLine(position))) )
      this->seekg(position);
    element = PeekFullLine();

    index = element.find_first_not_of(delimiters_);
    if (index != string::npos)
      {
	length = element.find_first_of(delimiters_, index);
	length = length==string::npos ? element.size() - index : length - index;
	element = element.substr(index, length);
      }
    else
      {
	index = element.size();
	length = 0;
	element = "";
      }

    this->seekg(index + length, ifstream::cur);

    return element;
  }

  //! Gets the next valid element.
  /*!
    Gets the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \param element (output) the next valid element.
  */
  template <class T>
  bool ExtStream::GetElement(T& element)
  {
    string str = GetElement();
    convert(str, element);

    return (str != "");
  }

  //! Gets the next valid element.
  /*!
    Gets the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \param element (output) the next valid element.
  */
  template <class T>
  bool ExtStream::GetRawElement(T& element)
  {
    string str = ExtStream::GetElement();
    convert(str, element);

    return (str != "");
  }

  //! Returns the next valid element without extracting it from the stream.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \return The next valid element.
  */
  string ExtStream::PeekElement()
  {
    streampos initial_position;
    string element;

    initial_position = this->tellg();
    iostate state = this->rdstate();

    element = GetElement();

    this->clear(state);
    this->seekg(initial_position);

    return element;
  }

  //! Gets the next valid element without extracting it from the stream.
  /*!
    Gets the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \param element (output) the next valid element.
  */
  template <class T>
  bool ExtStream::PeekElement(T& element)
  {
    streampos initial_position;
    bool success;

    initial_position = this->tellg();
    iostate state = this->rdstate();

    success = GetElement(element);

    this->clear(state);
    this->seekg(initial_position);

    return success;
  }

  //! Skips valid elements.
  /*!
    \param nb number of valid elements to be skipped.
  */
  void ExtStream::SkipElements(int nb)
  {
    for (int i=0; i<nb; i++)
      GetElement();
  }

  //! Returns the next valid number.
  /*!
    Returns the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \return The next valid number.
  */
  double ExtStream::GetNumber()
  {
    string element;
    while (GetElement(element) && !is_num(element));

    return is_num(element) ? to_num<double>(element) : 0.;
  }

  //! Gets the next valid number.
  /*!
    Gets the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \param element (output) the next valid number.
  */
  template <class T>
  bool ExtStream::GetNumber(T& number)
  {
    string element;
    bool success;
    while ((success = GetElement(element)) && !is_num(element));

    number = is_num(element) ? to_num<T>(element) : T(0);

    return success;
  }

  //! Returns the next valid number without extracting it from the stream.
  /*!
    Returns the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \return The next valid number.
  */
  double ExtStream::PeekNumber()
  {
    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    string element;
    while (GetElement(element) && !is_num(element));

    this->clear(state);
    this->seekg(initial_position);

    return is_num(element) ? to_num<double>(element) : 0.;
  }

  //! Gets the next valid number without extracting it from the stream.
  /*!
    Gets the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \param number (output) the next valid number.
  */
  template <class T>
  bool ExtStream::PeekNumber(T& number)
  {
    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    string element;
    bool success;
    while ((success = GetElement(element)) && !is_num(element));

    number = is_num(element) ? to_num<T>(element) : T(0);

    this->clear(state);
    this->seekg(initial_position);

    return success;
  }

  //! Skips numbers.
  /*!
    \param nb number of numbers to be skipped.
  */
  void ExtStream::SkipNumbers(int nb)
  {
    for (int i=0; i<nb; i++)
      this->GetNumber();
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) element following the variable name.
    \param name the name of the variable.
    \return the value of the variable.
  */
  string ExtStream::GetValue(string name)
  {
    searching_ = name;

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::GetValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    searching_ = "";

    return GetElement();
  }

  //! Gets the value of a given variable without extracting from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \return the value associated with the variable.
  */
  string ExtStream::PeekValue(string name)
  {
    streampos initial_position = this->tellg();
    iostate state = this->rdstate();    

    string element = this->GetValue(name);

    this->clear(state);
    this->seekg(initial_position);

    return element;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the (numerical) value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  template <class T>
  bool ExtStream::GetValue(string name, T& value)
  {
    searching_ = name;

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::GetValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    searching_ = "";

    return GetNumber(value);
  }

  //! Gets the value of a given variable without extracting them from the stream.
  /*!
    Gets the (numerical) value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  template <class T>
  bool ExtStream::PeekValue(string name, T& value)
  {
    searching_ = name;

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::PeekValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    bool success = GetNumber(value);

    this->clear(state);
    this->seekg(initial_position);

    searching_ = "";

    return success;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  bool ExtStream::GetValue(string name, string& value)
  {
    searching_ = name;

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::GetValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    searching_ = "";

    return GetElement(value);
  }

  //! Gets the value of a given variable without extracting them from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  bool ExtStream::PeekValue(string name, string& value)
  {
    searching_ = name;

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::PeekValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    bool success = GetElement(value);

    this->clear(state);
    this->seekg(initial_position);

    searching_ = "";

    return success;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value boolean associated with the variable.
  */
  bool ExtStream::GetValue(string name, bool& value)
  {
    searching_ = name;

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::GetValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    searching_ = "";

    return GetElement(value);
  }

  //! Gets the value of a given variable without extracting from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value boolean associated with the variable.
  */
  bool ExtStream::PeekValue(string name, bool& value)
  {
    searching_ = name;

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();

    string element;
    while (GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ExtStream::PeekValue: \"")
	+ name + string("\" not found in \"") + file_name_ + "\".";

    bool success = GetElement(value);

    this->clear(state);
    this->seekg(initial_position);

    searching_ = "";

    return success;
  }


  //////////////////
  // CONFIGSTREAM //
  //////////////////

  //! Default constructor.
  /*! Nothing is performed.
   */
  ConfigStream::ConfigStream(): ExtStream()
  {
    markup_tags_ = "<>$";
    section_ = "";
  }

  //! Main constructor.
  /*! Opens a file.
    \param file_name file to be opened.
  */
  ConfigStream::ConfigStream(string file_name,
			     string comments,
			     string delimiters,
			     string markup_tags):
    ExtStream(file_name, comments, delimiters),
    markup_tags_(markup_tags)
  {
    section_ = "";
  }

  //! Deselects the section.
  /*!
    Deselects the section (this is equivalent to SetSection("")).
   */
  void ConfigStream::NoSection()
  {
    section_ = "";
  }

  //! Sets the current section.
  /*!
    \param section current section.
  */
  void ConfigStream::SetSection(string section)
  {
    section_ = "";
    if (section != "")
      {
	this->FindFromBeginning(section);
	section_ = section;
      }
  }

  //! Returns the current section.
  /*!
    \return The current section.
  */
  string ConfigStream::GetSection() const
  {
    return section_;
  }

  //! Sets the markup tags.
  /*!
    \param markup_tags the new markup tags.
  */
  void ConfigStream::SetMarkupTags(string markup_tags)
  {
    markup_tags_ = markup_tags;
  }

  //! Returns the markup tags.
  /*!
    \return The markup tags.
  */
  string ConfigStream::GetMarkupTags() const
  {
    return markup_tags_;
  }

  //! Checks whether the stream or the current section is empty.
  /*!
    If no section has been selected, this method checks whether the stream has
    still valid elements to be read. If the stream is bound to a given
    section, this method checks whether there remains at least one element
    in the section.
    \return 'true' is the stream is empty, 'false' otherwise.
  */
  bool ConfigStream::IsEmpty()
  {
    streampos initial_position;
    bool success;

    initial_position = this->tellg();
    iostate state = this->rdstate();

    string element = ExtStream::GetElement();
    success = element != "";

    this->clear(state);
    this->seekg(initial_position);

    return !success || (section_ != "" && IsSection(element));
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found.
    \return true if the element was found, false otherwise.
    \note The scope of the search is only the current section if any.
  */
  bool ConfigStream::Find(string element)
  {
    this->searching_ = element;

    string elt;
    while (ExtStream::GetRawElement(elt) && elt!=element
	   && (section_ == "" || !IsSection(elt)));

    if (section_ != "" && (elt == "" || IsSection(elt)))
      throw string("Error in ConfigStream::Find: end of section \"")
	+ section_ + string("\" has been reached in file \"")
	+ this->file_name_ + string("\".\nUnable to find \"")
	+ element + "\".";
    if (elt == "")
      throw string("Error in ConfigStream::Find: \"")
	+ element + string("\" not found in \"") + this->file_name_ + "\".";

    this->searching_ = "";

    return elt == element;
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found from the beginning of the stream.
    \return true if the element was found, false otherwise.
    \note The current section (if any) is unset.
  */
  bool ConfigStream::FindFromBeginning(string element)
  {
    NoSection();
    this->Rewind();
    return this->Find(element);
  }

  //! Returns the next valid element.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \return The next valid element.
    \note Markups are replaced with their values.
  */
  string ConfigStream::GetElement()
  {
    string tmp;

    string element = ExtStream::GetElement();

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();    

    vector<string> elements;
    vector<bool> is_markup;

    split_markup(element, elements, is_markup, markup_tags_);

    element = "";

    for (int i = 0; i < int(elements.size()); i++)
      if (!is_markup[i])
	element += elements[i];
      else
	{
	  this->Rewind();
	  tmp = ExtStream::GetElement();
	  while (tmp != elements[i] && tmp != "")
	    tmp = ExtStream::GetElement();
	  if (tmp == "")
	    throw string("Error in ConfigStream::GetElement: the value of the markup \"")
	      + elements[i] + string("\" was not found in \"") + file_name_ + "\".";
	  element += this->GetElement();
	}

    this->clear(state);
    this->seekg(initial_position);

    if (!section_.empty() && (element == "" || IsSection(element)))
      {
	string message = string("End of section \"") + section_
	  + string("\" has been reached in file \"") + this->file_name_
	  + "\".";
	if (this->searching_ != "")
	  message += string("\nUnable to find \"")
	    + this->searching_ + string("\".");
	throw message;
      }

    return element;
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \return The next valid line.
  */
  string ConfigStream::GetLine()
  {
    string tmp;

    string element = ExtStream::GetLine();

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();    

    vector<string> elements;
    vector<bool> is_markup;

    split_markup(element, elements, is_markup, markup_tags_);

    element = "";

    for (int i = 0; i < int(elements.size()); i++)
      if (!is_markup[i])
	element += elements[i];
      else
	{
	  this->Rewind();
	  tmp = ExtStream::GetElement();
	  while (tmp != elements[i] && tmp != "")
	    tmp = ExtStream::GetElement();
	  if (tmp == "")
	    throw string("Error in ConfigStream::GetLine: the value of the markup \"")
	      + elements[i] + string("\" was not found in \"") + file_name_ + "\".";
	  element += this->GetElement();
	}

    this->clear(state);
    this->seekg(initial_position);

    return element;
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \param line (output) the next valid line.
  */
  bool ConfigStream::GetLine(string& line)
  {
    string tmp;

    bool success = ExtStream::GetLine(line);

    streampos initial_position = this->tellg();
    iostate state = this->rdstate();    

    vector<string> elements;
    vector<bool> is_markup;

    split_markup(line, elements, is_markup, markup_tags_);

    line = "";

    for (int i = 0; i < int(elements.size()); i++)
      if (!is_markup[i])
	line += elements[i];
      else
	{
	  this->Rewind();
	  tmp = ExtStream::GetElement();
	  while (tmp != elements[i] && tmp != "")
	    tmp = ExtStream::GetElement();
	  if (tmp == "")
	    throw string("Error in ConfigStream::GetLine: the value of the markup \"")
	      + elements[i] + string("\" was not found in \"") + file_name_ + "\".";
	  line += this->GetElement();
	}

    this->clear(state);
    this->seekg(initial_position);

    return success;
  }

  //! Checks whether a string is a section flag.
  /*!
    \param str string to be tested.
    \return True if 'str' is a section flag, false otherwise.
  */
  bool ConfigStream::IsSection(string str) const
  {
    return str[0] == '[' && str[str.size()-1] == ']';
  }


  ///////////////////
  // CONFIGSTREAMS //
  ///////////////////

  //! Default constructor.
  /*! Nothing is performed.
   */
  ConfigStreams::ConfigStreams():
    streams_(0), current_(streams_.begin()), section_(""), searching_("")
  {
  }

  //! Main constructor.
  /*! Opens a set of file.
    \param files files to be opened.
  */
  ConfigStreams::ConfigStreams(const vector<string>& files):
    streams_(files.size()), current_(streams_.begin()), section_(""),
    searching_("")
  {
    for (int i = 0; i < int(files.size()); i++)
      streams_[i] = new ConfigStream(files[i]);
  }

  //! Constructor.
  /*! Opens a file.
    \param file file to be opened.
  */
  ConfigStreams::ConfigStreams(string file):
    streams_(1), current_(streams_.begin()), section_(""), searching_("")
  {
    streams_[0] = new ConfigStream(file);
  }

  //! Constructor.
  /*! Opens two files.
    \param file0 first file to be opened.
    \param file1 second file to be opened.
  */
  ConfigStreams::ConfigStreams(string file0, string file1):
    streams_(2), current_(streams_.begin()), section_(""), searching_("")
  {
    streams_[0] = new ConfigStream(file0);
    streams_[1] = new ConfigStream(file1);
  }

  //! Constructor.
  /*! Opens three files.
    \param file0 first file to be opened.
    \param file1 second file to be opened.
    \param file2 third file to be opened.
  */
  ConfigStreams::ConfigStreams(string file0, string file1, string file2):
    streams_(3), current_(streams_.begin()), section_(""), searching_("")
  {
    streams_[0] = new ConfigStream(file0);
    streams_[1] = new ConfigStream(file1);
    streams_[2] = new ConfigStream(file2);
  }

  //! Destructor.
  ConfigStreams::~ConfigStreams()
  {
    for (current_ = streams_.begin(); current_ != streams_.end(); ++current_)
      delete (*current_);
  }

  //! Returns the vector of streams.
  /*!
    \return A reference to the vector of ConfigStream.
  */
  vector<ConfigStream*>& ConfigStreams::GetStreams()
  {
    return streams_;
  }

  //! Returns the current position in the vector of streams.
  /*!
    \return An iterator that points to the current element of the
    vector of streams.
  */
  vector<ConfigStream*>::iterator ConfigStreams::GetCurrent()
  {
    return current_;
  }
  
  //! Adds a file to the streams.
  /*!
    \param file file to be added.
  */
  void ConfigStreams::AddFile(string file)
  {
    unsigned int l = current_ - streams_.begin();
    streams_.push_back(new ConfigStream(file));
    current_ = streams_.begin() + l;
  }
  
  //! Deselects the section.
  /*!
    Deselects the section (this is equivalent to SetSection("")) and rewinds
    the stream.
   */
  void ConfigStreams::NoSection()
  {
    section_ = "";
    for (current_ = streams_.begin(); current_ != streams_.end(); ++current_)
      (*current_)->SetSection(section_);
  }

  //! Sets the current section.
  /*!
    \param section current section.
  */
  void ConfigStreams::SetSection(string section)
  {
    section_ = section;
    for (current_ = streams_.begin(); current_ != streams_.end(); ++current_)
      (*current_)->SetSection("");

    current_ = streams_.begin();
    bool found = false;
    try
      {
	(*current_)->SetSection(section);
	found = true;
      }
    catch (...)
      {
	(*current_)->Rewind();
	(*current_)->section_ = section_;
      }
    while (!found && current_ != streams_.end() - 1)
      {
	++current_;
	try
	  {
	    (*current_)->SetSection(section_);
	    found = true;
	  }
	catch (...)
	  {
	    (*current_)->Rewind();
	    (*current_)->section_ = section_;
	  }
      }

    if (!found)
      throw string("Error in ConfigStreams::SetSection: section \"")
	+ section + string("\" was not found in ") + FileNames() + ".";
  }

  //! Returns the current section.
  /*!
    \return The current section.
  */
  string ConfigStreams::GetSection() const
  {
    return section_;
  }

  //! Checks whether a line should be discarded.
  /*!
    \param line line to be checked.
  */
  bool ConfigStreams::Discard(string line) const
  {
    return (*current_)->Discard(line);
  }

  //! Skips discarded lines.
  /*!
    Extracts discarded lines.
    \return A reference to 'this'.
  */
  ConfigStreams& ConfigStreams::SkipDiscarded()
  {
    (*current_)->SkipDiscarded();
    while (current_ != streams_.end()-1 && !(*current_)->good())
      {
	++current_;
	(*current_)->SkipDiscarded();
      }
    return *this;
  }

  //! Skips delimiters.
  /*!
    Extracts following delimiters from the string, until another character
    is found.
    \return A reference to this.
  */
  ConfigStreams& ConfigStreams::SkipDelimiters()
  {
    (*current_)->SkipDelimiters();
    while (current_ != streams_.end()-1 && !(*current_)->good())
      {
	++current_;
	(*current_)->SkipDelimiters();
      }
    return *this;
  }

  //! Removes delimiters at both ends of a string.
  /*!
    Removes delimiters at the beginning and at the end of a string.
    \param 
  */
  string ConfigStreams::RemoveDelimiters(const string& str) const
  {
    return (*current_)->RemoveDelimiters(str);
  }

  //! Skips discarded lines and delimiters.
  /*!
    Extracts discarded lines and delimiters.
    \return A reference to this.
  */
  ConfigStreams& ConfigStreams::Skip()
  {
    this->SkipDiscarded();
    return this->SkipDelimiters();
  }

  //! Checks whether the streams are empty.
  /*!
    Checcks whether the streams have still valid elements to be read.
    \return 'true' is the streams are empty, 'false' otherwise.
  */
  bool ConfigStreams::IsEmpty()
  {
    return (*current_)->IsEmpty();
  }

  //! Rewinds all streams and goes back to the first stream.
  /*!
    Goes back to the beginning of each stream, clears the control state and
    goes back to the first stream.
    \return A reference to 'this'.
  */
  ConfigStreams& ConfigStreams::Rewind()
  {
    for (current_ = streams_.begin(); current_ != streams_.end(); ++current_)
      (*current_)->Rewind();
    current_ = streams_.begin();

    return *this;
  }

  //! Returns the next line.
  /*!
    \return The next line.
  */
  string ConfigStreams::GetFullLine()
  {
    string line;
    std::getline(**current_, line);

    if (is_empty(**current_) && current_ != streams_.end()-1)
      ++current_;

    return line;
  }

  //! Returns the next line.
  /*!
    \param line (output) the next line.
  */
  bool ConfigStreams::GetFullLine(string& line)
  {
    bool success = std::getline(**current_, line);

    if (is_empty(**current_) && current_ != streams_.end()-1)
      ++current_;

    return success;
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \return The next line.
  */
  string ConfigStreams::PeekFullLine()
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string line = (*current_)->PeekFullLine();

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return line;
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \param position (output) the position of the line following the next line.
    \return The next line.
  */
  string ConfigStreams::PeekFullLine(streampos& position)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string line = (*current_)->PeekFullLine(position);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return line;
  }

  //! Returns the next line without extracting it from the stream.
  /*!
    \param line (output) the next line.
  */
  bool ConfigStreams::PeekFullLine(string& line)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = (*current_)->PeekFullLine(line);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Skips full lines.
  /*!
    \param nb number of lines to be skipped.
  */
  void ConfigStreams::SkipFullLines(int nb)
  {
    for (int i = 0; i < nb; i++)
      this->GetFullLine();
  }

  //! Returns the next valid line, without markups substitution.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    No markup substitution is performed.
    \return The next valid line.
  */
  string ConfigStreams::GetRawLine()
  {
    this->Skip();
    return (*current_)->ExtStream::GetLine();
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \return The next valid line.
  */
  string ConfigStreams::GetLine()
  {
    string line;
    this->GetLine(line);
    return line;
  }

  //! Returns the next valid line.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    \param line (output) the next valid line.
  */
  bool ConfigStreams::GetLine(string& line)
  {
    string tmp;

    line = this->GetRawLine();
    bool success = (line != "");

    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    vector<string> elements;
    vector<bool> is_markup;

    split_markup(line, elements, is_markup, (*current_)->GetMarkupTags());

    line = "";

    for (int i = 0; i < int(elements.size()); i++)
      if (!is_markup[i])
	line += elements[i];
      else
	{
	  this->Rewind();
	  tmp = this->GetRawElement();
	  while (tmp != elements[i] && tmp != "")
	    tmp = this->GetRawElement();
	  if (tmp == "")
	    throw string("Error in ConfigStreams::GetLine: the value of the markup \"")
	      + elements[i] + string("\" was not found in ")
	      + FileNames() + ".";
	  line += this->GetElement();
	}

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \return The next valid line.
  */
  string ConfigStreams::PeekLine()
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string line = this->GetLine();

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return line;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \param position (output) the position of the line following the next valid line.
    \return The valid line.
  */
  string ConfigStreams::PeekLine(streampos& position)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string line = this->GetLine();
    position = (*current_)->tellg();

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return line;
  }

  //! Returns the next valid line without extracting it from the stream.
  /*!
    Returns the next valid line, i.e. the next line that is
    not a line to be discarded and from which comments have been extracted.
    Nothing is extracted from the stream.
    \param line (output) the next valid line.
  */
  bool ConfigStreams::PeekLine(string& line)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetLine(line);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Skips valid lines.
  /*!
    \param nb number of lines to be skipped.
  */
  void ConfigStreams::SkipLines(int nb)
  {
    for (int i = 0; i < nb; i++)
      this->GetLine();
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found.
    \return true if the element was found, false otherwise.
  */
  bool ConfigStreams::Find(string element)
  {
    searching_ = element;

    bool found;
    try
      {
	found = (*current_)->Find(element);
      }
    catch (...)
      {
	found = false;
      }
    while (!found && current_ != streams_.end()-1)
      {
	++current_;
	try
	  {
	    found = (*current_)->Find(element);
	  }
	catch (...)
	  {
	    found = false;
	  }
      }
    if (!found && !section_.empty())
      throw string("Error in ConfigStreams::Find: end of section \"")
	+ section_ + string("\" has been reached in ")
	+ FileNames() + string(".\nUnable to find \"")
	+ element + "\".";
    if (!found)
      throw string("Error in ConfigStreams::Find: \"")
	+ element + string("\" not found in ") + FileNames() + ".";

    searching_ = "";

    return found;
  }

  //! Sets the position of the get pointer after a given element.
  /*!
    Sets the position of the get pointer exactly after a given element.
    \param element the element to be found from the beginning of the stream.
    \return true if the element was found, false otherwise.
  */
  bool ConfigStreams::FindFromBeginning(string element)
  {
    NoSection();
    this->Rewind();
    return this->Find(element);
  }

  //! Returns the next valid element, without markups substitution.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded. No markup substitution is performed.
    \return The next valid element.
  */
  string ConfigStreams::GetRawElement()
  {
    string element;
    while ((element = (*current_)->ExtStream::GetElement()) == "" && current_ != streams_.end()-1)
      ++current_;
    return element;
  }

  //! Returns the next valid element.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \return The next valid element.
  */
  string ConfigStreams::GetElement()
  {
    string tmp;

    string element = GetRawElement();

    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    vector<string> elements;
    vector<bool> is_markup;

    split_markup(element, elements, is_markup, (*current_)->GetMarkupTags());

    element = "";

    for (int i = 0; i < int(elements.size()); i++)
      if (!is_markup[i])
	element += elements[i];
      else
	{
	  this->Rewind();
	  tmp = GetRawElement();
	  while (tmp != elements[i] && tmp != "")
	    tmp = GetRawElement();
	  if (tmp == "")
	    throw string("Error in ConfigStreams::GetElement: the value of the markup \"")
	      + elements[i] + string("\" was not found in ")
	      + FileNames() + ".";
	  element += this->GetElement();
	}

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    if (!section_.empty() && (element == "" || IsSection(element)))
      {
	string message = string("End of section \"") + section_
	  + string("\" has been reached in ") + FileNames() + ".";
	if (searching_ != "")
	  message += string("\nUnable to find \"")
	    + this->searching_ + string("\".");
	throw message;
      }

    return element;
  }

  //! Gets the next valid element.
  /*!
    Gets the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \param element (output) the next valid element.
  */
  template <class T>
  bool ConfigStreams::GetElement(T& element)
  {
    string str = this->GetElement();
    convert(str, element);

    return (str != "");
  }

  //! Returns the next valid element without extracting it from the stream.
  /*!
    Returns the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \return The next valid element.
  */
  string ConfigStreams::PeekElement()
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string element = this->GetElement();

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return element;
  }

  //! Gets the next valid element without extracting it from the stream.
  /*!
    Gets the next valid element, i.e. the next element that is
    not in a line to be discarded.
    \param element (output) the next valid element.
  */
  template <class T>
  bool ConfigStreams::PeekElement(T& element)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetElement(element);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Skips valid elements.
  /*!
    \param nb number of valid elements to be skipped.
  */
  void ConfigStreams::SkipElements(int nb)
  {
    for (int i = 0; i < nb; i++)
      this->GetElement();
  }

  //! Returns the next valid number.
  /*!
    Returns the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \return The next valid number.
  */
  double ConfigStreams::GetNumber()
  {
    string element;
    while (this->GetElement(element) && !is_num(element));

    return is_num(element) ? to_num<double>(element) : 0.;
  }

  //! Gets the next valid number.
  /*!
    Gets the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \param element (output) the next valid number.
  */
  template <class T>
  bool ConfigStreams::GetNumber(T& number)
  {
    string element;
    bool success;
    while ((success = this->GetElement(element)) && !is_num(element));

    number = is_num(element) ? to_num<T>(element) : T(0);

    return success;
  }

  //! Returns the next valid number without extracting it from the stream.
  /*!
    Returns the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \return The next valid number.
  */
  double ConfigStreams::PeekNumber()
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    double num = this->GetNumber();

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return num;
  }

  //! Gets the next valid number without extracting it from the stream.
  /*!
    Gets the next valid number, i.e. the next number that is
    not in a line to be discarded.
    \param number (output) the next valid number.
  */
  template <class T>
  bool ConfigStreams::PeekNumber(T& number)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetNumber(number);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Skips numbers.
  /*!
    \param nb number of numbers to be skipped.
  */
  void ConfigStreams::SkipNumbers(int nb)
  {
    for (int i = 0; i < nb; i++)
      this->GetNumber();
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) element following the variable name.
    \param name the name of the variable.
    \return the value of the variable.
  */
  string ConfigStreams::GetValue(string name)
  {
    searching_ = name;

    string element;
    while (this->GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ConfigStreams::GetValue: \"")
	+ name + string("\" not found in \"") + FileNames() + ".";

    searching_ = "";

    return this->GetElement();
  }

  //! Gets the value of a given variable without extracting from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \return the value associated with the variable.
  */
  string ConfigStreams::PeekValue(string name)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    string element = this->GetValue(name);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return element;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the (numerical) value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  template <class T>
  bool ConfigStreams::GetValue(string name, T& value)
  {
    searching_ = name;

    string element;
    while (this->GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ConfigStreams::GetValue: \"")
	+ name + string("\" not found in \"") + FileNames() + ".";

    searching_ = "";

    return GetNumber(value);
  }

  //! Gets the value of a given variable without extracting them from the stream.
  /*!
    Gets the (numerical) value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  template <class T>
  bool ConfigStreams::PeekValue(string name, T& value)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetValue(name, value);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  bool ConfigStreams::GetValue(string name, string& value)
  {
    searching_ = name;

    string element;
    while (this->GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ConfigStreams::GetValue: \"")
	+ name + string("\" not found in ") + FileNames() + ".";

    searching_ = "";

    return GetElement(value);
  }

  //! Gets the value of a given variable without extracting them from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value value associated with the variable.
  */
  bool ConfigStreams::PeekValue(string name, string& value)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetValue(name, value);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Gets the value of a given variable.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    \param name the name of the variable.
    \param value boolean associated with the variable.
  */
  bool ConfigStreams::GetValue(string name, bool& value)
  {
    searching_ = name;

    string element;
    while (this->GetElement(element) && element!=name);

    if (element != name)
      throw string("Error in ConfigStreams::GetValue: \"")
	+ name + string("\" not found in ") + FileNames() + ".";

    searching_ = "";

    return this->GetElement(value);
  }

  //! Gets the value of a given variable without extracting from the stream.
  /*!
    Gets the value of a given variable, i.e. the next valid
    (not in a discarded line) number or element following the variable name.
    Nothing is extracted from the stream.
    \param name the name of the variable.
    \param value boolean associated with the variable.
  */
  bool ConfigStreams::PeekValue(string name, bool& value)
  {
    vector<ConfigStream*>::iterator iter = current_;
    streampos initial_position = (*current_)->tellg();
    ifstream::iostate state = (*current_)->rdstate();

    bool success = this->GetValue(name, value);

    this->Rewind();
    current_ = iter;
    (*current_)->clear(state);
    (*current_)->seekg(initial_position);

    return success;
  }

  //! Checks whether a string is a section flag.
  /*!
    \param str string to be tested.
    \return True if 'str' is a section flag, false otherwise.
  */
  bool ConfigStreams::IsSection(string str) const
  {
    return str[0] == '[' && str[str.size()-1] == ']';
  }

  //! Returns file names in string form.
  /*!
    \return File names in order to print them on screen.
  */
  string ConfigStreams::FileNames() const
  {
    string output = "";
    int Nstream = streams_.size();
    for (int i = 0; i < Nstream - 2; i++)
      output += string("\"") + streams_[i]->GetFileName() + "\", ";
    if (Nstream > 1)
      output += string("\"") + streams_[Nstream - 2]->GetFileName()
	+ "\" or ";
    if (Nstream > 0)
      output += string("\"") + streams_[Nstream - 1]->GetFileName() + "\"";
    return output;
  }

}  // namespace Talos.


#define TALOS_FILE_FILES_CXX
#endif
