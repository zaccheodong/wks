var k_shell;
k_shell = {};

(function () {
    k_shell.getbkimage = function (cb) {
        native function getBKImage();
        return getBKImage(cb);
    };


 k_shell.appquit = function () {
        native function appQuit();
        return appQuit();
    };
}
)();