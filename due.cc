/* Franco 18/08/2009: take userid and a list of "similar" projects */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



using namespace std;

int main (int argc, char** argv) {

  if ( argc < 2 ) {
    cout << "Error, not enough arguments" << endl;
    exit(2);
  }

  int myguy = atoi(argv[1]);

  string mydata="../data.txt";

  string line;
  ifstream myfile;

  string fi, se;
  int first, second;

  set<int> users, projects;

  map <int,set<int> > up; // user -> projects

  map <  int , double > sim; 
// similarity of user in argv[1] with the rest of the users

  map < double, vector < int > > multisim; 
  // A map similarity -> vector of users, so that I can sort similarity


  myfile.open(mydata.c_str());

  if (myfile.is_open())
  {
    while (! myfile.eof() ) {
      // reading the file line by line, splitting the line in user and
      // project, then adding user and project to appropriate sets.
      getline(myfile,line);
      fi = line.substr(0,line.find(":"));
      se = line.substr(line.find(":")+1);

      first = atoi(fi.c_str());
      second = atoi(se.c_str());

      //      cout << first << " " << second << endl;

      users.insert(first);
      projects.insert(second);

      if ( (up.find(first)) == NULL ) {
	// new user
	set <int> tmpset;
	tmpset.insert(second);
	up[first] = tmpset;
      } else {
	up[first].insert(second);
      }
    }

    myfile.close();

  } 
  // I assume similarities are in a file called tmp.txta
  mydata = "tmp.txta";
  myfile.open(mydata.c_str());

  if (myfile.is_open())
  {
    while (! myfile.eof() ) {
      // reading the file line by line, splitting the line in user and
      // project, then adding user and project to appropriate sets.
      getline(myfile,line);
      if ( line.size() > 0 ) {
	fi = line.substr(0,line.find(":"));
	se = line.substr(line.find(":")+1);
	
	first = atoi(fi.c_str());
	//      second = strtod(se.c_str(),NULL);
	double mysec = atof(se.c_str());
	
      
	// Let's try to compute similarities
	sim[first] = mysec;
	multisim[mysec].push_back(first);
      }
    }

    

    map <double, vector < int > >::reverse_iterator upi;
    // I'm taking them in reverse order so that high similarity is first.

    vector < int > result;
    
    // maybe here: check those that are watched by all the guys with
    // max similarity.

    for(upi = multisim.rbegin(); upi != multisim.rend(); upi++) {
      vector < int >::iterator it;
      for ( it = (*upi).second.begin(); it != (*upi).second.end();
	    it++ ) {
	//projects by the friend not by the guy
	vector < int > tmpvector;
	set_difference(up[*it].begin(),up[*it].end(),up[myguy].begin(),
		       up[myguy].end(),insert_iterator<vector < int >
		       > (tmpvector, tmpvector.begin()));
	if ( tmpvector.size() > 0 ) {
	  if ( ( result.size() + tmpvector.size() ) < 11 ) {
	    // Add all the projects from the friend to the guy
	    for ( int i=0; i < tmpvector.size(); i++) {
	      if ( find(result.begin(),result.end(),tmpvector[i]) ==
		   result.end() ) { 
		result.push_back(tmpvector[i]);
	      }
	    }
	  } else {
	    for ( int i=0; i<(10-result.size()); i++ ) {
	      if ( find(result.begin(),result.end(),tmpvector[i]) ==
		   result.end() ) {
		result.push_back(tmpvector[i]);
	      }	    
	    // only copy up to 10
	    }	  
	  }
	}
      }
    }

    sort(result.begin(),result.end());
    cout << "RESULT " << myguy << ":";
    vector < int >::iterator ti;
    int count = 0;
    for ( ti = result.begin(); ti != result.end(); ti++ ) {
      cout << (*ti);
      if ( count != result.size()-1 ) {
	cout << ",";
      }
      count ++;
    }
    cout << endl;
      
    cout << "DONE: for  user " << myguy << " I found " << result.size() << endl;
    myfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}
