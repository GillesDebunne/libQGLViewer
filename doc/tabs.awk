BEGIN { p=0; inli=0; search=""; }

/<div/ { if (p>1) p=p+1; }
/<div class=\"tabs/ { p=p+1; } # matches tabs and tabs2

/href=\"index.html\"/ { if (p==1) { gsub("index.html", "../index.html"); main=$0; } }

# Extract the search li. inli = recording lines in li to "search"
/<li/ { if ((p==1) && (search=="")) inli=1; }

/SearchBox/ { if (inli==1) { searchFound=1; } }

/<\/li>/ {
if (inli==1) {
  if (searchFound==1) {
    search=search"\n"$0;
    searchFound=0;
    #print "RES=",search
  } else {
    #print "RESET SEARCH";
    search="";
  }
  inli=0;
}
}

/<li/ { if ((p==2) && (main!="")) {print main; main="";} }

/<li.*>All.*<\/li>/ { next }
/<li.*>Variables.*<\/li>/ { next }
/<li.*>Related.*Functions.*<\/li>/ { next }

/<\/ul>/ { if (p==2) { print search; } }

/<div class=\"nav\">/ { next; }
/namespaceqglviewer.html/ { next; }

{
 if (inli==1) {search=search"\n"$0;}
 if (p==1) { next; }
 print;
}
