def iterate_module_tree(mm):
    iterator = mm.flat_tree_begin()

    while iterator != mm.flat_tree_end():
        yield iterator.get_ref()
        iterator.advance()





def print_dot_tree(mm):
    s = " digraph graphname {\n"

    s += "   nodeMaster[shape=box, label=\"INPUT\"]\n"

    for node in iterate_module_tree(mm):
        s += "  node{}[shape=box, label=\"{} [{}]\\n{} v{}\"]\n".format(node.id,
                                                                        node.id,
                                                                        node.modulekey,
                                                                        node.minfo.name,
                                                                        node.minfo.version)
        if node.parentid == 0:
            s += "  nodeMaster -> node{}\n".format(node.id)
        else:
            s += "  node{} -> node{}\n".format(node.parentid, node.id)

    s += "}" # close the graph
    return s

