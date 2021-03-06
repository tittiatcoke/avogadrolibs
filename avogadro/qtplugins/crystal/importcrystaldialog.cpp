/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2016 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "importcrystaldialog.h"
#include "ui_importcrystaldialog.h"

#include <avogadro/core/molecule.h>
#include <avogadro/io/poscarformat.h>
#include <avogadro/qtplugins/openbabel/obfileformat.h>

#include <QDebug>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <QtGui/QClipboard>

using std::vector;
using std::string;

namespace Avogadro {
namespace QtPlugins {

ImportCrystalDialog::ImportCrystalDialog(QWidget *p) :
  QDialog(p),
  m_ui(new Ui::ImportCrystalDialog)
{
  m_ui->setupUi(this);
}

ImportCrystalDialog::~ImportCrystalDialog()
{
  delete m_ui;
}

bool ImportCrystalDialog::importCrystalClipboard(Avogadro::Core::Molecule &mol)
{
  QString text = QApplication::clipboard()->text();
  m_ui->edit_text->setText(text);
  // If the user rejected, just return false
  if (this->exec() == QDialog::Rejected)
    return false;

  // Use POSCAR format by default. If the extension was set, use that instead
  std::string ext = m_ui->edit_extension->text().toUpper().toStdString();

  // Update the text
  text = m_ui->edit_text->toPlainText();
  std::stringstream s(text.toStdString());

  if (ext.empty() || ext == "POSCAR") {
   Avogadro::Io::PoscarFormat format;
   if (format.read(s, mol))
     return true;
   // Print out the error messages if we failed
   else
     qDebug() << QString::fromStdString(format.error());
  }
  // Try using the extension
  else {
    // We don't need to set any of the info, so just put in empty strings for
    // most arguments (except the extension)
    vector<string> fileExtensions, mimeTypes;
    fileExtensions.push_back(ext);
    OBFileFormat ob("", "", "", "", fileExtensions, mimeTypes);
    if (ob.read(s, mol))
      return true;
    // Print out the error messages from the read if we failed
    else
      qDebug() << QString::fromStdString(ob.error());
  }
  displayInvalidFormatMessage();
  return false;
}

void ImportCrystalDialog::displayInvalidFormatMessage()
{
  QMessageBox::critical
    (this,
     tr("Cannot Parse Text"),
     tr("The input is either not formatted using\n"
        "VASP format or the obabel conversion failed.\n\n"));
  reject();
  close();
}

} // namespace QtPlugins
} // namespace Avogadro
