def MakeInfo(minfo, bpcore):
  op = bpcore.OptionMap()
  for o in minfo["options"]:
    op.Set(o[0], o[1], o[2])

  mi = minfo
  mi["options"] = op
  return mi

