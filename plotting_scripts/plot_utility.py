import ROOT
import math
import sys
import cmsstyle as CMS


def dphi(dPhi):
    if dPhi < math.pi: return dPhi
    else: return 2*math.pi - dPhi

def setHist(hist, color):
    hist.Scale(1./hist.Integral())
    hist.SetLineWidth(2)
    hist.SetLineColor(color)
    hist.SetMarkerStyle(8);
    hist.SetMarkerColor(color)

class twoHist:
    def __init__(self, name, nbin, low, high, sample1='', sample2='', color=3):
        self.color = color
        self.name = name
        self.nbin = nbin
        self.low = low
        self.high = high
        self.hist1 = ROOT.TH1F("h1_"+name, "", nbin, low, high)
        self.hist2 = ROOT.TH1F("h2_"+name, "", nbin, low, high)
        self.sample1 = sample1
        self.sample2 = sample2
    def Fill1(self, entry, weight):
        self.hist1.Fill(entry, weight)
    def Fill2(self, entry, weight):
        self.hist2.Fill(entry, weight)
    def makePlot(self, cha):
        setHist(self.hist1, 2)
        setHist(self.hist2, self.color)
        max1 = self.hist1.GetBinContent(self.hist1.GetMaximumBin())
        max2 = self.hist2.GetBinContent(self.hist2.GetMaximumBin())
        self.hist1.GetYaxis().SetRangeUser(0, 1.05 * max(max1,max2))
        self.hist1.GetXaxis().SetTitle(self.name)
        leg = ROOT.TLegend(.6,.7,.9,.85)
        leg.SetBorderSize(1)
        #leg.SetFillColor(10)
        #leg.SetFillStyle(0)
        leg.SetTextFont(42)
        leg.SetTextSize(0.035)
        leg.AddEntry(self.hist1, self.sample1, "L")
        leg.AddEntry(self.hist2, self.sample2, "L")

        ratio = ROOT.TH1D("ratio", "ratio", self.nbin, self.low, self.high)
        ratio.Divide(self.hist1, self.hist2)
        ratio.SetMarkerColor(2)
        ratio.SetMarkerStyle(8)
        ratio.SetMarkerSize(1)
        ratio.SetTitle("")
        ratio.GetYaxis().SetRangeUser(ratio.GetMinimum() / 1.05, ratio.GetMaximum() * 1.05)
        ratio.GetYaxis().SetNdivisions(4)
        ratio.GetXaxis().SetTitleOffset(0.9)
        ratio.GetXaxis().SetTitleSize(0.15)
        ratio.GetXaxis().SetLabelSize(0.12)
        ratio.GetYaxis().SetTitleSize(0.15)
        ratio.GetYaxis().SetLabelSize(0.12)
        ratio.GetYaxis().SetTitleOffset(0.2)
        ratio.GetYaxis().SetTitleSize(0.15)
        ratio.GetYaxis().SetTitle("Ratio")
        ratio.GetXaxis().SetTitle(self.name)

        line = ROOT.TLine(self.low, 1, self.high, 1)
        line.SetLineColor(ROOT.kBlack)
        line.SetLineStyle(7)
        line.SetLineWidth(2)
        can = ROOT.TCanvas("c","c", 800, 800)
        ROOT.gStyle.SetOptStat(0)
        """
        CMS.SetExtraText("Simulation")
        CMS.SetCmsText("CMS")
        CMS.SetLumi("")
        CMS.SetCmsTextSize(0.5)"""
        #can = CMS.cmsCanvas('', 0, 0, 0, 0, '', '', square = CMS.kSquare, extraSpace=0.01, iPos=0)
        can.Divide(1,2)
        can.cd(1)
        ROOT.gPad.SetPad(0.0, 0.3, 1.0, 1.0)
        ROOT.gPad.SetBottomMargin(0)
        ROOT.gPad.SetTopMargin(0.15)
        self.hist1.Draw()
        self.hist2.Draw("SAME")
        leg.Draw("SAME")
        can.cd(2)
        ROOT.gPad.SetPad(0.0, 0.1, 1.0, 0.3)
        ROOT.gPad.SetTopMargin(0)
        ROOT.gPad.SetBottomMargin(0.3)
        ratio.Draw()
        line.Draw("same")
        can.cd(0)
#        CMS.CMS_lumi(can, 0, 0.5)
        can.Draw()
        can.SaveAs("plots/" + self.name + "_" + cha + ".pdf")

class threeHist:
    def __init__(self, name, nbin, low, high, sample1='', sample2='', sample3='', color1=3, color2=4, color3=806):
        self.color = color
        self.name = name
        self.nbin = nbin
        self.low = low
        self.high = high
        self.hist1 = ROOT.TH1F("h1_"+name, "", nbin, low, high)
        self.hist2 = ROOT.TH1F("h2_"+name, "", nbin, low, high)
        self.sample1 = sample1
        self.sample2 = sample2
    def Fill1(self, entry, weight):
        self.hist1.Fill(entry, weight)
    def Fill2(self, entry, weight):
        self.hist2.Fill(entry, weight)
    def makePlot(self, cha):
        setHist(self.hist1, 2)
        setHist(self.hist2, self.color)
        max1 = self.hist1.GetBinContent(self.hist1.GetMaximumBin())
        max2 = self.hist2.GetBinContent(self.hist2.GetMaximumBin())
        self.hist1.GetYaxis().SetRangeUser(0, 1.05 * max(max1,max2))
        self.hist1.GetXaxis().SetTitle(self.name)
        leg = ROOT.TLegend(.6,.7,.88,.8)
        leg.SetBorderSize(1)
        #leg.SetFillColor(10)
        #leg.SetFillStyle(0)
        leg.SetTextFont(42)
        leg.SetTextSize(0.03)
        leg.AddEntry(self.hist1, self.sample1, "L")
        leg.AddEntry(self.hist2, self.sample2, "L")

        ratio = ROOT.TH1D("ratio", "ratio", self.nbin, self.low, self.high)
        ratio.Divide(self.hist1, self.hist2)
        ratio.SetMarkerColor(2)
        ratio.SetMarkerStyle(8)
        ratio.SetMarkerSize(1)
        ratio.SetTitle("")
        ratio.GetYaxis().SetRangeUser(ratio.GetMinimum() / 1.05, ratio.GetMaximum() * 1.05)
        ratio.GetXaxis().SetTitleOffset(0.9)
        ratio.GetXaxis().SetTitleSize(0.15)
        ratio.GetXaxis().SetLabelSize(0.13)
        ratio.GetYaxis().SetTitleSize(0.1)
        ratio.GetYaxis().SetLabelSize(0.1)
        ratio.GetYaxis().SetTitleOffset(0.2)
        ratio.GetYaxis().SetTitleSize(0.15)
        ratio.GetYaxis().SetTitle("Ratio")
        ratio.GetXaxis().SetTitle(self.name)

        line = ROOT.TLine(self.low, 1, self.high, 1)
        line.SetLineColor(ROOT.kBlack)
        line.SetLineStyle(7)
        line.SetLineWidth(2)
        can = ROOT.TCanvas("c","c", 800, 800)
        ROOT.gStyle.SetOptStat(0)
        CMS.SetExtraText("Preliminary")
        CMS.SetCmsText("CMS")
        CMS.SetLumi("")
        CMS.SetCmsTextSize(0.5)
        #can = CMS.cmsCanvas('', 0, 0, 0, 0, '', '', square = CMS.kSquare, extraSpace=0.01, iPos=0)
        can.Divide(1,2)
        can.cd(1)
        ROOT.gPad.SetPad(0.0, 0.3, 1.0, 1.0)
        ROOT.gPad.SetBottomMargin(0)
        ROOT.gPad.SetTopMargin(0.15)
        self.hist1.Draw("HIST")
        self.hist2.Draw("HIST SAME")
        leg.Draw("SAME")
        can.cd(2)
        ROOT.gPad.SetPad(0.0, 0.15, 1.0, 0.3)
        ROOT.gPad.SetTopMargin(0)
        ROOT.gPad.SetBottomMargin(0.3)
        ratio.Draw()
        line.Draw("same")
        can.cd(0)
        CMS.CMS_lumi(can, 0, 0.5)
        can.Draw()
        can.SaveAs("plots/" + self.name + "_" + cha + ".pdf")
