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


#ifndef TALOS_FILE_FILES_HXX


namespace Talos
{

  bool exists(string file_name);
  unsigned long file_size(string file_name);
  unsigned long stream_size(istream& stream);
  bool is_empty(istream& stream);
  bool has_element(istream& stream);

  //! Extended streams.
  class ExtStream: public ifstream
  {
  protected:
    //! File name associated with the stream.
    string file_name_;
    //! Characters that denote a comment line.
    string comments_;
    //! Characters considered as delimiters.
    string delimiters_;

    //! Field currently searched.
    string searching_;

  public:
    ExtStream();
    ExtStream(string file_name,
	      string comments = "#%",
	      string delimiters = " \t:=|\n,;\r\x0D\x0A");

    virtual ~ExtStream();

    bool Discard(string line) const;
    ExtStream& SkipDiscarded();

    void SetDelimiters(string delimiters);
    void SetComments(string comments);

    string GetDelimiters() const;
    string GetComments() const;
    string GetFileName() const;

    ExtStream& SkipDelimiters();
    string RemoveDelimiters(const string& str) const;

    ExtStream& Skip();

    void Open(string file_name, openmode mode = in);
    void Close();
    
    bool IsEmpty();
    
    ExtStream& Rewind();

    string GetFullLine();
    bool GetFullLine(string& line);
    string PeekFullLine();
    string PeekFullLine(streampos& position);
    bool PeekFullLine(string& line);
    void SkipFullLines(int nb);

    virtual string GetLine();
    virtual bool GetLine(string& line);
    virtual string PeekLine();
    virtual string PeekLine(streampos& position);
    virtual bool PeekLine(string& line);
    void SkipLines(int nb);

    bool Find(string element);
    bool FindFromBeginning(string element);

    virtual string GetElement();
    template <class T>
    bool GetElement(T& element);
    template <class T>
    bool GetRawElement(T& element);
    string PeekElement();
    template <class T>
    bool PeekElement(T& element);
    void SkipElements(int nb);

    double GetNumber();
    template <class T>
    bool GetNumber(T& number);
    double PeekNumber();
    template <class T>
    bool PeekNumber(T& number);
    void SkipNumbers(int nb);

    string GetValue(string name);
    string PeekValue(string name);

    template <class T>
    bool GetValue(string name, T& value);
    template <class T>
    bool PeekValue(string name, T& value);

    bool GetValue(string name, string& value);
    bool PeekValue(string name, string& value);

    bool GetValue(string name, bool& value);
    bool PeekValue(string name, bool& value);
  };

  //! Streams associated with configuration files.
  class ConfigStream: public ExtStream
  {
  protected:
    string markup_tags_;
    string section_;
    
  public:
    ConfigStream();
    ConfigStream(string file_name,
		 string comments = "#%",
		 string delimiters = " \t:=|\n,;",
		 string markup_tags = "<>$");

    void NoSection();
    void SetSection(string section);
    string GetSection() const;

    void SetMarkupTags(string markup_tags);
    string GetMarkupTags() const;

    bool IsEmpty();

    bool Find(string element);
    bool FindFromBeginning(string element);

    virtual string GetElement();

    virtual string GetLine();
    virtual bool GetLine(string& line);

  private:
    bool IsSection(string str) const;

    friend class ConfigStreams;
  };

  //! Streams associated with several configuration files.
  class ConfigStreams
  {
  protected:
    vector<ConfigStream*> streams_;
    vector<ConfigStream*>::iterator current_;

    string section_;

    //! Field currently searched.
    string searching_;

  public:
    ConfigStreams();
    ConfigStreams(const vector<string>& files);
    ConfigStreams(string file0);
    ConfigStreams(string file0, string file1);
    ConfigStreams(string file0, string file1, string file2);

    ~ConfigStreams();

    vector<ConfigStream*>& GetStreams();
    vector<ConfigStream*>::iterator GetCurrent();

    void AddFile(string file);

    void NoSection();
    void SetSection(string section);
    string GetSection() const;

    bool Discard(string line) const;
    ConfigStreams& SkipDiscarded();

    ConfigStreams& SkipDelimiters();
    string RemoveDelimiters(const string& str) const;

    ConfigStreams& Skip();

    bool IsEmpty();
    
    ConfigStreams& Rewind();

    string GetFullLine();
    bool GetFullLine(string& line);
    string PeekFullLine();
    string PeekFullLine(streampos& position);
    bool PeekFullLine(string& line);
    void SkipFullLines(int nb);

    string GetRawLine();
    string GetLine();
    bool GetLine(string& line);
    string PeekLine();
    string PeekLine(streampos& position);
    bool PeekLine(string& line);
    void SkipLines(int nb);

    bool Find(string element);
    bool FindFromBeginning(string element);

    string GetRawElement();
    string GetElement();
    template <class T>
    bool GetElement(T& element);
    string PeekElement();
    template <class T>
    bool PeekElement(T& element);
    void SkipElements(int nb);

    double GetNumber();
    template <class T>
    bool GetNumber(T& number);
    double PeekNumber();
    template <class T>
    bool PeekNumber(T& number);
    void SkipNumbers(int nb);

    string GetValue(string name);
    string PeekValue(string name);

    template <class T>
    bool GetValue(string name, T& value);
    template <class T>
    bool PeekValue(string name, T& value);

    bool GetValue(string name, string& value);
    bool PeekValue(string name, string& value);

    bool GetValue(string name, bool& value);
    bool PeekValue(string name, bool& value);

  private:
    bool IsSection(string str) const;
    string FileNames() const;
  };

}  // namespace Talos.


#define TALOS_FILE_FILES_HXX
#endif
