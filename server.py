import cherrypy
from ROOT import TCanvas, TPad, TFormula, TF1, TPaveLabel, TH1F, TFile, TTree
from ROOT import gROOT, gBenchmark, THttpServer, TFile, gROOT, AddressOf
from array import array
import random
import json
import threading
sitelist = []

def sitereset():
    global sitelist
    threading.Timer(10.0, sitereset).start()
    sitelist = []

class MyMonitoring(object):

    @cherrypy.expose
    def whichsites(self):
        return json.dumps(sitelist)

    @cherrypy.expose
    def index(self):
        return open('monitor.html')

    # USAGE:
    # http://localhost:8080/getdata?site=fermilab&latitude=1&longitude=2&altitude=3&timestamp=4
    @cherrypy.expose
    def getdata(self, site,latitude,longitude,altitude,timestamp):
        mystruct.fLat   = float(latitude)
        mystruct.fLong   = float(longitude)
        mystruct.fAlt   = float(altitude)
        mystruct.fTimestamp   = int(timestamp)
        mystruct.fSite   = site

        # Real-time site list
        if mystruct.fSite in sitelist:
            print("empty")
        else:
            sitelist.append(mystruct.fSite)

        h1rate.Fill(float(altitude))
        tree.Fill()
        f.Write()

f = TFile( 'raw.root', 'UPDATE' )
gROOT.ProcessLine(
    "struct MyStruct {\
    Float_t     fLat;\
    Float_t     fLong;\
    Float_t     fAlt;\
    Int_t     fTimestamp;\
    Char_t    fSite[64];\
};" );
from ROOT import MyStruct
mystruct = MyStruct()
sitereset()

tree = TTree( 'rawdata', 'Rawdata' )
tree.Branch( 'lat', mystruct, 'Lat/F' )
tree.Branch( 'long', mystruct, 'Long/F' )
tree.Branch( 'alt', mystruct, 'Alt/F' )
tree.Branch( 'timestamp', mystruct, 'Timestamp/I' )
tree.Branch( 'site', AddressOf( mystruct, 'fSite' ), 'Site/C' )

h1rate = TH1F( 'h1rate', 'Cumulative rate distribution', 100, 0, 20 )
f.Write()
cherrypy.quickstart(MyMonitoring())
