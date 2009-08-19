/* Franco 17/08/2009: compute similarity */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>


using namespace std;

int main () {

  // Building the set of users, the set of projects, the map
  // user -> project list and the similarity measure
  string mydata="../data.txt";

  string line;
  ifstream myfile;

  string fi, se;
  int first, second;

  set<int> users, projects;

  map <int,set<int> > up; // user -> projects

  map < pair < int, int >, double > sim; 
// similarity: <user1,user2> -> value

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


    cout << "There are " << users.size() << "  users" << endl;
    cout << "There are " << projects.size() << "  projects" << endl;
    
    // Let's try to compute similarities

    vector<int> uservector(users.begin(),users.end());


    for (int i = 0; i < uservector.size(); i++ ) {
      for ( int j = i+1; j < uservector.size(); j++) {
	  set<int> tmp;

	  // Computing the intersection between up[i] and up[j]:
	  set_intersection(up[i].begin(), up[i].end(), up[j].begin(),
			   up[j].end(), insert_iterator< set < int > >
			   (tmp,tmp.begin()) );
	  
	  // Similarity: size of intersection / size of larger set.
	  
	 
	  


	  if ( tmp.size() != 0 ) {

	    pair<int,int> mypair (uservector[i],uservector[j]);

	    // COmputing similarity as defined above.
	    double mysim = (double)tmp.size() / ((double)
						 max(up[uservector[i]].size(),
						     up[uservector[j]].size()));


	    sim[mypair] = mysim;

	    cout << uservector[i] << "," << uservector[j] << ":"
		 << mysim << endl;
	    
// 	    cout << uservector[i] << " and " << uservector[j] << 
// 	      " are a bit similar:" << endl; 

// 	    set <int>::iterator si;

// 	    cout << uservector[i] << ": [ ";
// 	    for ( si = (up[uservector[i]]).begin(); si !=
// 		    (up[uservector[i]]).end() ; si++ ) { 
// 	      cout << (*si) << " ";
// 	    }
// 	    cout << "]" << endl;

// 	    cout << uservector[j] << ": [ ";
// 	    for ( si = (up[uservector[j]]).begin(); si !=
// 		    (up[uservector[j]]).end() ; si++ ) { 
// 	      cout << (*si) << " ";
// 	    }
// 	    cout << "]" << endl;
	    
// 	    cout << "Intersection: ";
// 	    for ( si =tmp.begin(); si != tmp.end() ; si++ ) { 
// 	      cout << (*si) << " ";
// 	    }
// 	    cout << endl << endl;
	  }
      }
    }


//     cout << "NOW PRINTING" << endl;

//     map <int,set<int> >::iterator upi;
//     set <int>::iterator si;

//     for(upi = up.begin(); upi != up.end(); upi++) {
//       cout << (*upi).first << " [ ";
//       for ( si = (upi->second).begin(); si != (upi->second).end() ;
// 	    si++ ) {
// 	cout << (*si) << " ";
//       }
//       cout << "]" << endl;
//     }

  }

  else cout << "Unable to open file"; 

  return 0;
}
