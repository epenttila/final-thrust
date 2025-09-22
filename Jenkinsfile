pipeline {
    agent none
    stages {
        stage('Build Windows') {
            agent {
                label 'windows'
            }
            environment {
                PRESET = 'msvc-release'
            }
            steps {
                script {
                    def vswhereExe = "C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe"

                    def vsInstallPath = powershell(
                        script: "& '${vswhereExe}' -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
                        returnStdout: true
                    ).trim()

                    if (!vsInstallPath) {
                        error "No Visual Studio installation found (vswhere + fallbacks failed)"
                    }

                    def vcvarsall = "${vsInstallPath}\\VC\\Auxiliary\\Build\\vcvarsall.bat"
                    if (!fileExists(vcvarsall)) {
                        error "Could not find vcvarsall.bat at: ${vcvarsall}"
                    }

                    env.VC_VARSALL = vcvarsall
                    echo "Using vcvarsall: ${vcvarsall}"
                }
                bat """
                    call "%VC_VARSALL%" amd64
                    cmake --preset %PRESET%
                    cmake --build build/%PRESET% --target package
                """
                archiveArtifacts artifacts: "build/${env.PRESET}/*.zip", fingerprint: true
            }
            post {
                cleanup {
                    cleanWs()
                }
            }
        }
        stage('Build Linux') {
            agent {
                dockerfile {
                    filename 'Dockerfile.linux'
                }
            }
            environment {
                PRESET = 'gcc-release'
            }
            steps {
                sh 'cmake --preset ${PRESET}'
                sh 'cmake --build build/${PRESET} --target package'
                archiveArtifacts artifacts: "build/${env.PRESET}/*.zip", fingerprint: true
            }
            post {
                cleanup {
                    cleanWs()
                }
            }
        }
        stage('Build Android') {
            agent {
                dockerfile {
                    filename 'Dockerfile.android'
                }
            }
            environment {
                KEYSTORE = credentials('release-keystore')
                KEYSTORE_PASSWORD = credentials('release-keystore-password')
            }
            steps {
                dir('android') {
                    sh 'gradlew assembleRelease'
                }
                archiveArtifacts artifacts: 'android/app/build/outputs/apk/*/*.apk', fingerprint: true
            }
            post {
                cleanup {
                    cleanWs()
                }
            }
        }
    }
}
