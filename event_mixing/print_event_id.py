import ROOT
import sys

file0 = ROOT.TFile.Open("../HZg_AOD/pico/DY_output_kingscanyon_v1_2017_merge.root", "READ")
tree0 = file0.Get("tree")
file1 = ROOT.TFile.Open("../HZg_AOD/pico/no_match_DY_full2017.root", "READ")
tree1 = file1.Get("tree")

listfile1 = open('ggf.txt', 'w')
listfile2 = open('vbf.txt', 'w')
nEntries0 = tree0.GetEntries()
nEntries1 = tree1.GetEntries()
no_match = []
for j in range(nEntries1):
    tree1.GetEntry(j)
    no_match.append(tree1.event)
for i in range(nEntries0):
    tree0.GetEntry(i)
    if not any(tree0.event == x for x in no_match):
        continue
    if tree0.njet <= 1:
        print ("eventId = ", tree0.event, file = listfile1)
        print ("-"*13, file = listfile1)

    else:
        print ("eventId = ", tree0.event, file = listfile2)
        print ("-"*13, file = listfile2)
