#include "SIDT.h"

void printUsage () {
  printf ("Use SIDT (Site Identification Decision Tree) to learn and scan sites in .edg files.\n");
  printf ("Usage : ./SIDT [options] [command]\n");
  printf ("Commands are :\n");
  printf ("        -h or --help : print this help\n");
  printf ("        -l or --learn fileToLearn [fileToScan] : learns a .edg file [and scans a .edg file]\n");
  printf ("        -ll or --learn-list listToLearn [listToScan] : learns a list of .edg files and scans a list of .edg files\n");
  printf ("A list of .edg files has a path to a .edg file per line, for instance :\n");
  printf ("        firefox.edg\n");
  printf ("        ../samples/sality.edg\n");
  printf ("        StuxDuqu/duqu.edg\n");
  printf ("Options are :\n");
  printf ("        -ncl or --no-check-labels : do not check the symbols (labels) of sites\n");
  printf ("        -ss or --site-size siteSize : specifies the wanted size of the sites (default: 24)\n");
  printf ("        -s or --stats : prints stats about the decision tree\n");
  printf ("        -o or --output-dt [path] : outputs the decision tree as a .dot file for display\n");
  printf ("        -lc or --limit-site-count limit : specifies how many sites should be learnt in total (for debug purposes, default: unlimited)\n");
  printf ("Examples :\n");
  printf ("        ./SIDT -ss 16 -l firefox.edg firefox.edg : learns and scans a single file with sites of size 16\n");
  printf ("        ./SIDT  -lc 30 -o DTfirefox.dot -l firefox.edg : learns only 30 sites from a single file and outputs the decision tree as a .dot file\n");
  printf ("        ./SIDT -s -ll malwares.lst sample.lst : learns a list of malwares and scans a list of files, then prints stats about the decision tree\n");
}

int main (int argc, char *argv[]) {
  optionFuncs = 0;

  if (argc <= 2) {
    printUsage ();
    return 0;
  }

  FILE *fpPattern = NULL;
  char *pathPattern;
  FILE *fpTest = NULL;
  char *pathTest;
  char optionLearn = 0;
  char optionLearnList = 0;
  char learnOk = 0;
  char scanOk = 0;
  char optionTest = 0;
  char optionShowStats = 0;
  char optionOutputDt = 0;
  char *pathDT;
  int siteCountLimit = 0;
  int siteSize = 24;
  char withLabels = 1;
  bool optionInfo = false;

  ProgInfo *scanInfo = NULL;

  int a;
  for (a = 1; a < argc; a++) {
    if (strcmp (argv[a], "-h") == 0 || strcmp (argv[a], "-help") == 0 || strcmp (argv[a], "--help") == 0) {
      printUsage ();
      return 0;
    }
    else if (strcmp (argv[a], "-l") == 0 || strcmp (argv[a], "--learn") == 0) {
      optionLearn = 1;
    }
    else if (strcmp (argv[a], "-ll") == 0 || strcmp (argv[a], "--learn-list") == 0) {
      optionLearnList = 1;
    }
    else if (strcmp (argv[a], "-s") == 0 || strcmp (argv[a], "--stats") == 0) {
      optionShowStats = 1;
    }
    else if (strcmp (argv[a], "-ncl") == 0 || strcmp (argv[a], "-ncs") == 0 || strcmp (argv[a], "--no-check-labels") == 0) {
      withLabels = 0;
    }
    else if (strcmp (argv[a], "-o") == 0 || strcmp (argv[a], "--output-dt") == 0) {
      optionOutputDt = 1;
      if (a < argc - 1) {
        pathDT = argv[a + 1];
        a++;
      }
      else {
        printf ("No file specified to output the DT, default is dt_output.dot.\n");
        pathDT = (char *) "dt_output.dot";
      }
    }
    else if (strcmp (argv[a], "-lc") == 0 || strcmp (argv[a], "--limit-site-count") == 0) {
      if (a < argc - 1) {
        siteCountLimit = atoi (argv[a + 1]);
        a++;
      }
      else {
        printf ("Need number to limit site count\n");
        printUsage ();
        return 1;
      }
    }
    else if (strcmp (argv[a], "-ss") == 0 || strcmp (argv[a], "--site-size") == 0) {
      if (a < argc - 1) {
        siteSize = atoi (argv[a + 1]);
        a++;
      }
      else {
        printf ("Need number to specify site size\n");
        printUsage ();
        return 1;
      }
    }
    else {
      if (!learnOk) {
        fpPattern = fopen (argv[a], "r");

        if (fpPattern == NULL) {
          printf ("Can't open pattern graph %s\n", argv[a]);
          return 1;
        }
        pathPattern = argv[a];
        learnOk = 1;
      }
      else if (!scanOk) {
        optionTest = 1;
        fpTest = fopen (argv[a], "r");

        if (fpTest == NULL) {
          printf ("Can't open test graph %s\n", argv[a]);
          return 1;
        }
        pathTest = argv[a];
        scanOk = 1;
      }

      else {
        optionFuncs = 1;
        scanInfo = progFuncFromFile (argv[a]);
//                              printf("%p %p %p\n", scanInfo, scanInfo->functions, *scanInfo->functions[0]->name);
//                              return 0;
      }
    }
  }

  if (fpPattern == NULL) {
    printf ("Not file to learn.\n");
    printUsage ();
    return 1;
  }

  char valence = 2;
  int n = 0;
  int n_pattern = 0;
  int n_test = 0;
  decisionTree *dt = newDecisionTree (siteSize, valence);

  if (optionLearn) {
    graph_t *gr;
    graph_from_file (&gr, fpPattern);
    fclose (fpPattern);

    char *msg = (char *) malloc ((strlen (pathPattern) + 1) * sizeof (char));
    sprintf (msg, "%s", pathPattern);
    n_pattern = learnGraph (gr, dt, msg, siteCountLimit, withLabels);
    printf ("%d sites learned from %s\n", n_pattern, msg);
    graph_free (gr);

    if (optionTest) {
      graph_from_file (&gr, fpTest);
      fclose (fpTest);

      int *countByProg = (int *) calloc (dt->nProgs, sizeof (int));
      int *scanFuncs;
      if (optionFuncs)
        scanFuncs = (int *) calloc (scanInfo->n, sizeof (int));
      n = findGraph (gr, dt, countByProg, scanInfo, scanFuncs, withLabels);
      printf ("%d sites of %s found from %s\n", n, pathTest, dt->progs[0]->message);
      printf ("Pattern graph %s has %d sites ; test graph %s has %d sites found matching.\n", pathPattern, n_pattern, pathTest, n);
      if (optionFuncs) {
        printf ("\nFunctions found: \n");
        int k;
        for (k = 0; k < scanInfo->n; k++) {
          if (scanFuncs[k] != 0) {
            printf ("%d %s\n", scanFuncs[k], scanInfo->functions[k]->name);
          }
        }
      }
      if (optionFuncs)
        free (scanFuncs);
      free (countByProg);
      graph_free (gr);
    }
  }
  else if (optionLearnList) {
    // learning :
    if (optionInfo)
      printf ("Learning :\n");
    char line[128];
    FILE *fp;
    graph_t *gr;
    int i = 0;
    while (fgets (line, sizeof (line), fpPattern) != NULL && (siteCountLimit == 0 || dt->nLearntSites < siteCountLimit)) {
//       if (siteCountLimit == 0 || siteCountLimit < dt->nLearntSites){
      i++;
      line[strlen (line) - 1] = 0;
      fp = fopen (line, "r");
      if (fp == NULL) {
        if (optionInfo)
          fprintf (stderr, "%d, ERROR: Can't open graph to learn %s\n", i, line);
        continue;
      }

      char a = graph_from_file (&gr, fp);
      fclose (fp);
      if (a != 0) {
        if (optionInfo)
          fprintf (stderr, "%d, This file is not a valid graph: %s\n", i, line);
        continue;
      }

      char *msg = (char *) malloc ((strlen (line) + 1) * sizeof (char));
      sprintf (msg, "%s", line);
      n = learnGraph (gr, dt, msg, siteCountLimit, withLabels);
      if (optionInfo)
        printf ("%d, %s: %d sites learned\n", i, msg, n);
      graph_free (gr);
//       }
    }
    fclose (fpPattern);
//              read();
//              scanf();

    if (optionInfo)
      printf ("%d unique sites.\n", dt->nLearntSites);

    if (optionTest) {
      // scanning :
      if (optionInfo)
        printf ("\nDetection :\n");
      char line[128];
      i = 0;
      int k = 0;
      while (fgets (line, sizeof (line), fpTest) != NULL) {
        i++;
        line[strlen (line) - 1] = 0;
        fp = fopen (line, "r");
        if (fp == NULL) {
          if (optionInfo)
            fprintf (stderr, "%d, ERROR: Can't open graph to test %s\n", i, line);
          continue;
        }

        char a = graph_from_file (&gr, fp);
        fclose (fp);
        if (a != 0) {
          if (optionInfo)
            fprintf (stderr, "%d, This file is not a valid graph: %s\n", i, line);
          continue;
        }

        int *countByProg = (int *) calloc (dt->nProgs, sizeof (int));
        int *scanFuncs;
        if (optionFuncs)
          scanFuncs = (int *) calloc (scanInfo->n, sizeof (int));
        n = findGraph (gr, dt, countByProg, scanInfo, scanFuncs, withLabels);
        if (optionInfo)
          printf ("%d, %s: %d sites found\n", i, line, n);
        for (k = 0; k < dt->nProgs; k++) {
          if (countByProg[k] != 0) {    //TODO >=30 sinon faux positif ??
            printf ("   %d from %s (%d sites)\n", countByProg[k], dt->progs[k]->message, dt->progs[k]->nSites);
          }
        }

        if (optionFuncs) {
          if (optionInfo)
            printf ("\nFunctions found: \n");
          for (k = 0; k < scanInfo->n; k++) {
            if (scanFuncs[k] != 0) {
              printf ("%d %s\n", scanFuncs[k], scanInfo->functions[k]->name);
            }
          }
        }

        if (optionFuncs)
          free (scanFuncs);
        free (countByProg);
        graph_free (gr);
      }
      fclose (fpTest);
    }
  }

  if (optionOutputDt) {
    char *dotstr = drawDT (dt, withLabels);
    FILE *fpp = fopen (pathDT, "w");
    if (fpp == NULL) {
      fprintf (stderr, "Cannot open %s\n", pathDT);
    }
    fprintf (fpp, "Digraph {\n%s}", dotstr);
    fclose (fpp);
  }

  if (optionShowStats) {
    printInfoDT (dt);
  }

//      while(0==0){
//
//      }

  if (optionFuncs)
    freeProgFunctions (scanInfo);
  if (optionInfo)
    freeDecisionTree (dt);      // TODO: à enlever, c'est juste pour les tests en performance
}
