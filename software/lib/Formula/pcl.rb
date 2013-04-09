require 'formula'

class Pcl < Formula
  homepage 'http://www.pointclouds.org'
  url 'http://svn.pointclouds.org/pcl/trunk'
  sha1 ''

  depends_on 'cmake'
  depends_on 'boost' :universal
  depends_on 'eigen'
  depends_on 'flann' => 'enable-python'
  depends_on 'cminpack'
  depends_on 'vtk' => ['qt', 'python']
  depends_on 'qhull'
  depends_on 'libusb' :universal
  depends_on 'doxygen'
  depends_on 'sphinx'

  def install
    system "mkdir", "build"
    Dir.chdir 'build' do
      system "cmake", ".."
      system "make"
      system "make", "install"
    end
  end

  test do
    # `test do` will create, run in and delete a temporary directory.
    #
    # This test will fail and we won't accept that! It's enough to just replace
    # "false" with the main program this formula installs, but it'd be nice if you
    # were more thorough. Run the test with `brew test pcl`.
    system "false"
  end
end
