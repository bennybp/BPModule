# Main namespace
from . import modulemanager

def IterateModuleTree(mm):
    iterator = mm.FlatTreeBegin()

    while iterator != mm.FlatTreeEnd():
        yield iterator.GetRef()
        iterator.Advance()





def PrintDotTree(mm):
    s = " digraph graphname {\n"

    for node in IterateModuleTree(mm):
        s += "  node{}[shape=box, label=\"{} [{}]\\n{} v{}\"]\n".format(node.id,
                                                                        node.id,
                                                                        node.modulekey,
                                                                        node.minfo.name,
                                                                        node.minfo.version)
        if node.parentid != 0:
            s += "  node{} -> node{}\n".format(node.parentid, node.id)

    s += "}" # close the graph
    return s



