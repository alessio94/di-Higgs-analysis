CAFlwtnn
=====================
*Requirements*: 
CAFCore (use the most up-to-date master branch until CAFCore 19.05 is released), 
AnalysisBase (`lwtnn` is included in AnalysisBase version >21.2.4)

This package provides a rather minimal wrapper around lwtnn (https://github.com/lwtnn/lwtnn). Currently only simple serial evaluations (i.e., no RNNs) are supported by this wrapper.

Setup
--------------
The `TQlwtnnObservable` comes with a factory that is registered to CAFCore dynamically
uppon loading `libCAFlwtnn`. This can be automated by adding `libCAFlwtnn` to the `CAF_PRELOAD_LIBRARIES` environment variable.
After running cmake on this package (or any package that in turn includes this one) a setup script is auto-generated which will be automatically called by CAFCore's setup script.
Typically this should all happen automatically when source'ing your regular analysis setup script (usually: `source myAnalysis/setup/setupAnalysis.sh`) *after* running cmake.

Hence, all that is left to do is to
- clone this package inside your analysis package (better: adding it as a submodule),
- execute cmake, and
- source your usual analysis setup script


Usage
-------------
The Observable is automatically instantiated when encountering an expression of the form

```
lwtnn(path/to/definition.json, {nameOfOutputNode1,nameOfOutputNode2})
```
for `json` files that were converted for use with the "Functional API" (or `LightweightGraph` in `lwtnn` class parlance). An arbitrary number of output nodes (but at least one) can be specified. If multiple ones are specified the Observable will act as a vector Observable with indices corresponding to the ordering of output node names given in the expression. 

To use a `json` file with a layout for the "Sequential API" (or `LightweightNeuralNetwork` in `lwtnn` class parlance) use
```
lwtnnSeq(path/to/definition.json, {nameOfOutputNode1,nameOfOutputNode2})
```


The json file passed to `lwtnn` needs to be slightly modified w.r.t. what `lwtnn`s converter scripts produce from, e.g., Keras models. The Observable expressions corresponding to the inputs to be passed to `lwtnn` have to be included in the name of the inputs as in the following examples:

Functional:
```
"inputs": [
    {
      "name": "node_0",
      "variables": [
        {
          "name": "energy0=Event$(cand)[0].part(0).e()*0.001",
          "offset": -1.299909432742597,
          "scale": 0.009114703901093474
        },
        ...
```
Sequential:
```
 "inputs": [
        {
          "name": "energy0=Event$(cand)[0].part(0).e()*0.001",
          "offset": -1.299909432742597,
          "scale": 0.009114703901093474
        },
        ...
```
The names before the `=` should be kept short for performance reasons, everything after the first `=` is treated as the Observable expression. A full example is provided as `example_neural_net.json` and example_neural_net_.
