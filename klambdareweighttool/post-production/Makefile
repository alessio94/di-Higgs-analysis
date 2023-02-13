Target  = klambda_HEFT_reweight HEFT_reweight klambda_reweight BM_HEFT_reweight
Objects = Root.o NLO_EFT.o
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)

all:$(Target)


HEFT_reweight: HEFT_reweight.cxx $(Objects)
	g++ -std=gnu++11  -o $@ HEFT_reweight.cxx $(Objects) $(ROOTFLAGS) $(ROOTLIBS)

klambda_reweight: klambda_reweight.cxx klambda_weight.o
	g++ -o $@ klambda_reweight.cxx klambda_weight.o $(ROOTFLAGS) $(ROOTLIBS)

klambda_HEFT_reweight: klambda_HEFT_reweight.cxx $(Objects)
	g++ -std=gnu++11  -o $@ klambda_HEFT_reweight.cxx $(Objects) $(ROOTFLAGS) $(ROOTLIBS)

BM_HEFT_reweight: BM_HEFT_reweight.cxx $(Objects)
	g++ -std=gnu++11  -o $@ BM_HEFT_reweight.cxx $(Objects) $(ROOTFLAGS) $(ROOTLIBS)

# src/ 

Root.o: src/Root.cxx src/Root.h
	g++ -std=gnu++11  -g -o $@ -c src/Root.cxx $(ROOTFLAGS)

NLO_EFT.o: src/NLO_EFT.cxx src/NLO_EFT.h
	g++ -std=gnu++11  -g -o $@ -c src/NLO_EFT.cxx 


klambda_weight.o: src/klambda_weight.cxx src/klambda_weight.h
	g++ -std=gnu++11 -g -o $@ -c src/klambda_weight.cxx $(ROOTFLAGS)

clean:
	rm -f HEFT_reweight klambda_reweight klambda_HEFT_reweight *.o
