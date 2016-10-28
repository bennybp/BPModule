###############################################################
# General Notes
#
# We import the C++ GeneralException as GeneralException_
# and then create a new python class which contains a
# GeneralException_ as a member.
#
# This is done so that GeneralException (the python version)
# can derive from (python) Exception. This class cannot
# derive from both GeneralException_ and Exception because
# of some intrinsic limitation of python (cannot multiply-inherit
# from a python class and a C++ class).
#
# Either way, this is somewhat elegant as it stands
###############################################################

class GeneralException(Exception):
  """A base exception for Pulsar (python version)

     An exception in Pulsar is basically a string with some fancy formatting.
     There is one line (an overall description) followed by pairs of
     information. This is all available via the what() member (or by casting
     this object to a string), which will return this information as a
     nicely-formatted manner.
  """

  def __init__(self, whatstr, *exinfo):
    """Construct a GeneralException

       The extra information passed to this function via \p exinfo must be
       in the form "key1, value1, key2, value2, ...". Both the keys
       and the values must be string or convertible to strings.

    Args:
        whatstr: Some string describing the exception
        exinfo: Additional information to add to the exception
    Raises:
        None
    """

    self.__gex = GeneralException_(whatstr)
    self.append_info(*exinfo)


  def append_info(self, *exinfo):
    """Add information to this exception object

       The extra information passed to this function via \p exinfo must be
       in the form "key1, value1, key2, value2, ...". Both the keys
       and the values must be string or convertible to strings.

    Args:
        exinfo: Additional information to add to the exception
    Raises:
        None
    """

    # Convert the star-args to a list
    lst = list(exinfo)

    # Check to see if the list has an even number of elements.
    # If not, append a big warning instead. We don't want to
    # raise an exception since that may override the one you are
    # trying to throw.
    if len(lst) % 2:
      lst.append("ERROR: I WAS NOT PASSED AN EVEN NUMBER OF EXTRA INFO")

    # Split the star-args/list into pairs and give it to my
    # (C++) GeneralException_
    plist = [ lst[i:i+2] for i in range(0, len(lst), 2) ]
    for i in plist:
      self.__gex.append_info(str(i[0]), str(i[1]))


  def what(self):
    """Return all the information as a string

       The string will be formatted for output, with the description on
       its own line and each key,value pair on its own line.
    """
    return self.__gex.what()


  def __str__(self):
    """Return all the information as a string

       The string will be formatted for output, with the description on
       its own line and each key,value pair on its own line.
    """
    return self.__gex.what()

