from java.io import File
from java.lang import Runnable
from javafx.application import Platform
from javafx.stage import DirectoryChooser
from org.csstudio.display.builder.representation.javafx.widgets import JFXBaseRepresentation
from org.csstudio.display.builder.runtime.script import ScriptUtil
from org.phoebus.ui.docking import DockStage
from org.phoebus.ui.javafx import Screenshot


logger = ScriptUtil.getLogger()
display = widget.getTopDisplayModel()

class SaveAllScreenshots(Runnable):
    def __init__(self, display):
        self.display = display

    def get_screenshot_dir(self, window):
        chooser = DirectoryChooser()
        chooser.setTitle("Select folder for screenshots")
        return chooser.showDialog(window)
    
    def get_savepath(self, title, savedir):
        n = 0
        while True:
            savepath = File(savedir, "%s.%d.png" % (self.escape_filename(title), n))
            if not savepath.exists():
                return savepath
            n += 1

    def escape_filename(self, title):
        title = title.strip()
        for char in '\\/:*?"<>| ':
            title = title.replace(char, "_")
        return title
        
    def run(self):
        window = JFXBaseRepresentation.getJFXNode(self.display).getScene().getWindow()

        savedir = self.get_screenshot_dir(window)
        if savedir is None: return

        for stage in DockStage.getDockStages():
            for pane in DockStage.getDockPanes(stage):
                for item in pane.getDockItems():
                    app = item.getApplication()
                    tab_pane = item.getTabPane()
                    node = app.getRepresentation().getModelParent()
                    title = item.getLabel()

                    tab_pane.getSelectionModel().select(item)
                    tab_pane.requestLayout()
                    tab_pane.applyCss()
                    tab_pane.layout()

                    savepath = self.get_savepath(title, savedir)
                    try:
                        Screenshot(node).writeToFile(savepath)
                        logger.info("Saved screenshot {} -> {}".format(title, savepath.getAbsolutePath()))
                    except Exception as e:
                        logger.error("Failed to save {}: {}".format(title, e))

Platform.runLater(SaveAllScreenshots(display))