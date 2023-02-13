
# uhepp plotter -- should be separate module
import re
import os
import json
from collections import OrderedDict
from datetime import datetime
import ROOT
import QFramework
from QFramework import WARN, INFO

DEFAULT_API = "https://uhepp.org/api/"

try:
    import requests
except ImportError:
    pass

class ThinProcess:
    """Think wrapper to provide a process name"""
    def __init__(self, name):
        self.name = name
    def GetName(self):
        return self.name

class Plotter:
    """
    Duck-typed uhepp plotter
    
    To use this plotter and push plot to a central hub, add the following
    lines to the visualize config.

    ```
    plotFormats: json
    plotter: uhepp
    # Collection id, e.g. 1
    plotter.uhepp.collection: 1
    ```

    The environment variables UHEPP_API and UHEPP_TOKEN are honored.
    """
    def __init__(self, samples):
        """Create a new plotter object"""
        self.samples = samples
        self.clearProcesses()

    def setNormalizationInfo(self, path_to_normalization):
        """Set normalization info"""
        #  not actually used
        self.path_to_normalization = path_to_normalization

    def loadSystematics(self, systematics_band):
        """Set systematics band"""
        # not used at the moment
        self.systematics_band = systematics_band

    def setScheme(self, hist_scheme):
        """Set histogram scheme"""
        #  not actually used
        self.hist_scheme = hist_scheme

    def getReader(self):
        return self.samples

    def clearProcesses(self):
        """Clear internal list of processes""" 
        self.processes = OrderedDict()

    def removeProcessesByName(self, pattern):
        """Not yet supported"""

    def importProcessesFromFile(self, hist_processes):
        """Read the process definition file"""
        hist_processes = str(hist_processes)
        with open(hist_processes) as process_file:
            for i, line in enumerate(process_file):
                line = line.strip()
                if not line or line.startswith("#"):
                    # Skip empty lines
                    continue
                tags = QFramework.TQTaggable()
                tags.importTags(line)
                name = tags.getTagStringDefault(".name", tags.getTagStringDefault(".path", "p%d" % i))
                path = tags.getTagStringDefault(".path", "--")
                if str(path) in ["|", "||"]:
                    continue
                name = str(name)
                self.processes[name] = tags
        return True

    def getListOfProcessNames(self):
        """Return a list of process objects"""
        return [ThinProcess(name) for name in self.processes]

    def getProcessPath(self, process_name):
        """Return the path of a process"""
        return self.processes[process_name].getTagStringDefault(".path", "")

    def getSystematics(self, channel):
        """Not supported"""
        return False

    def get_processes_with_tag(self, tag_name):
        """Return ordered dict of processes which have the given tag"""
        return OrderedDict(
            (name, process) for name, process in self.processes.items()
            if process.getTagDefault(tag_name, False)
        )

    def get_process_color(self, process, cut_histname, tags):
        """Extract the color of a process"""
        # Check if set explicitly in process file
        fill_color = str(process.getTagDefault("histFillColor", ""))
        line_color = str(process.getTagDefault("histLineColor", ""))
        if fill_color or line_color:
            return self.resolve_color(fill_color or line_color)

        # Use default fall-back from style tags in sample folder
        path = process.getTagDefault(".path", "/")
        path = self.resolve_path(path, tags)
        th1 = self.samples.getHistogram(path, cut_histname)
        fill_color = th1.GetFillColor()
        line_color = th1.GetLineColor()
        color = fill_color or line_color or None

        return self.resolve_color(color)

    @staticmethod
    def resolve_color(color):
        """Convert ROOT colors (kRed+1) to HTML string"""
        if not color:
            return None
        if isinstance(color, int):
            value = color
        else:
            tokens = re.split("([+-])", color)
            value = 0
            mode = 1
            for token in tokens:
                if token == "+":
                    continue
                if token == "-":
                    mode *= -1
                    continue
                try:
                    token = int(token)
                    value += mode * token
                    mode = 1
                    continue
                except ValueError:
                    pass
                value += mode * getattr(ROOT, token.strip())
                mode = 1

        color = ROOT.gROOT.GetColor(value) 
        if not color:
            return None
        red = color.GetRed()
        green = color.GetGreen()
        blue = color.GetBlue()

        rgb = [red, green, blue]
        rgb = [int(255 * c) for c in rgb]
        rgb = ["%02x" % c for c in rgb]
        rgb = "".join(rgb)
        return "#" + rgb

    def process2yield(self, process, cut_hist, tags):
        """Exract the histogram from the sample folder and convert to Yield"""

        path = process.getTagDefault(".path", ".")
        scale = process.getTagDefault("scale", 1.0)

        path = self.resolve_path(path, tags)
        base_th1 = self.samples.getHistogram(path, cut_hist)
        bin_count = base_th1.GetNbinsX()

        def extract(th1, accessor=lambda x: x.GetBinContent):
            return [accessor(th1)(i) * scale for i in range(bin_count + 2)]

        base = extract(base_th1)
        stat = extract(base_th1, lambda x: x.GetBinError)

        return dict(base=base, stat=stat)

    def resolve_path(self, path, tags):
        """Replace tags and aliases in the path"""
        aliases = QFramework.TQTaggable()
        aliases.setTagString("lepch", tags.getTagStringDefault("input.lepch", "?"))
        aliases.setTagString("channel", tags.getTagStringDefault("input.channel", "?"))
        aliases.setTagString("eatachannel", tags.getTagStringDefault("input.datachannel", "?"))
        aliases.importTagsWithoutPrefix(tags, "alias.")
        aliases.importTagsWithoutPrefix(tags, "input.")

        return aliases.replaceInText(path)


    @staticmethod
    def tex(text):
        """Convert ROOT pseudo-tex to actual tex"""
        text = str(text)
        text = text.replace("#it", "").replace("#", "\\")
        if set("\\{}^_") & set(text) and "$" not in text:
            return "$%s$" % text
        return text

    def process2stack(self, processes, cut_histname, tags, bartype="stepfilled"):
        """Convert a dict of processes into a stack and return the stack dict"""
        return {
            "type": bartype,
            "error": "stat",
            "content": [
                {
                    "yield": [process_name],
                    "label": self.tex(process_name),
                    "style": {
                        "color": self.get_process_color(process, cut_histname, tags)
                    }
                }
                for process_name, process in processes.items()
            ]
        }

    def plotAndSaveAs(self, cut_histname, file_path, tags):
        """Create uhepp JSON file and push it to central hub"""
        cut_name, histname = cut_histname.split("/")
        random_process = next(iter(self.processes.values()))
        random_path = random_process.getTagDefault(".path", ".")
        random_path = self.resolve_path(random_path, tags)
        random_th1 = self.samples.getHistogram(random_path, cut_histname)
        x_label = random_th1.GetXaxis().GetTitle()
        x_label = str(x_label)
        match = re.search("^(.*) \[([^\]]+)\]$", x_label)
        if match:
            variable = match.group(1)
            unit = match.group(2)
        else:
            variable = x_label
            unit = None

        # Build stacks
        stacks = []

        bkg = self.get_processes_with_tag(".isBackground")
        yields = {}
        if bkg:
            stacks.append(self.process2stack(bkg, cut_histname, tags))
            for process_name, process in bkg.items():
                yields[process_name] = self.process2yield(process, cut_histname, tags)

        sig = self.get_processes_with_tag(".isSignal")
        if sig:
            stacks.append(self.process2stack(sig, cut_histname, tags, bartype="step"))
            for process_name, process in sig.items():
                yields[process_name] = self.process2yield(process, cut_histname, tags)

        data = self.get_processes_with_tag(".isData")
        if data:
            stacks.append(self.process2stack(data, cut_histname, tags, bartype="points"))
            for process_name, process in data.items():
                yields[process_name] = self.process2yield(process, cut_histname, tags)

        filename = []

        channel = str(tags.getTagDefault("input.channel", ""))
        if channel:
            filename.append(channel)

        filename.append(str(cut_name))
        filename.append(str(histname))

        data_obj = {
            "version": "0.1",
            "type": "histogram",
            "metadata": {
                "date": datetime.now().isoformat(),
                "filename": "-".join(filename),
                "producer": "CAF",
                "event_selection": str(cut_name),
                "tags": {
                    "cut": str(cut_name),
                    "channel": channel,
                }
            },
            "badge": {
                "brand": "ATLAS",
                "label": str(tags.getTagDefault("labels.atlas", "")),
                "subtext": self.tex(tags.getTagDefault("labels.process", "")),
            },
            "variable": {
                "symbol": self.tex(variable),
                "log": tags.getTagDefault("style.logScale", False)
            },
            "bins": {
                "edges": self.get_th1_edges(random_th1),
                "include_underflow": tags.getTagDefault("style.showUnderflow", False),
                "include_overflow": tags.getTagDefault("style.showOverflow", False),
            },
            "stacks": stacks,
            "yields": yields,
            "ratio": [],
        }
        if unit:
            data_obj["variable"]["unit"] = unit

        if tags.getTagDefault("style.showRatio", True) and data and bkg:
            data_obj["ratio"] = [
               {
                   "type": "points",
                   "error": "stat",
                   "numerator": [str(name) for name in data],
                   "denominator": [str(name) for name in bkg],
               }
            ]

            if tags.getTagDefault("style.forceRatioLimits", False):
                data_obj["ratio_axis"] = {
                   "min": tags.getTagDefault("style.ratioMin", 0.4),
                   "max": tags.getTagDefault("style.ratioMax", 1.6),
                }

            if tags.hasTag("geometry.sub.height"):
                data_obj["layout"] = {
                    "ratio_fraction": tags.getTagDefault("geometry.sub.height", 0.25)
                }

        # Write to file
        with open(file_path, "w") as file_obj:
            json.dump(data_obj, file_obj)
        
        #@env: [UHEPP_API] address of the UHEPP API, defaults to https://uhepp.org/api/
        api_url = str(tags.getTagDefault(
            "uhepp.api",
            os.environ.get("UHEPP_API", DEFAULT_API))
        )
        #@env: [UHEPP_TOKEN] token to authenticate against the UHEPP API
        api_key = os.environ.get("UHEPP_TOKEN", None)
        collection_id = str(tags.getTagDefault("uhepp.collection", ""))

        if collection_id:
            response = self.push(data_obj, api_url, api_key, collection_id)
            if response.ok:
                links = requests.utils.parse_header_links(response.headers["Link"])
                links = [link for link in links if link["rel"] == "ui"]
                if links:
                    url = links[0]["url"]
                else:
                    url = api_url

                INFO("  pushed to %s" % url)
            else:
                status_code = response.status_code
                error = str(response.text )
                WARN("  pushing failed (status code %d): %s" % (status_code, error))

        return True

    @staticmethod
    def push(data, api_url, api_key, collection_id):
        """Push data to central hub"""
        headers = {}
        if api_key is not None:
            headers["Authorization"] = "Token " + api_key

        post_data = {
            'uhepp': data,
            'collection': api_url + "collections/" + collection_id + "/"
        }

        response = requests.post(api_url + "plots/",
                                 json=post_data,
                                 headers=headers)
        return response
    
    @staticmethod
    def get_th1_edges(th1):
        """Extract and return yields (incl. under/over flow) from TH1"""
        x_axis = th1.GetXaxis()
        n_bins = x_axis.GetNbins()
        return [x_axis.GetBinLowEdge(i + 1) for i in range(n_bins)] \
               + [x_axis.GetBinUpEdge(n_bins)]
